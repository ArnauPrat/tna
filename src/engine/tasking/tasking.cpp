
#include "../data/queue.h"
#include "../types.h"
#include "atomic_counter.h"
#include "mutex.h"
#include "task_pool.h"
#include "tasking.h"
#include <boost_context/fcontext.h>

#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>



namespace tna
{

struct execution_context_t;

using execution_context_queue_t = queue_t<execution_context_t*>;

static size_t m_stack_size = 0;

/**
 * \brief Stores the context of an execution
 */
struct execution_context_t
{
  char* p_stack = nullptr;
  boost_context::fcontext_t m_context = nullptr;
};


void
execution_context_init(execution_context_t* exec_context)
{
  assert(m_stack_size != 0 && "STACK SIZE must be greater than zero");
  void *ptr;
	int result = posix_memalign(&ptr, getpagesize(), m_stack_size + 2*getpagesize());
  assert( result == 0 && "Failed aligned memory allocation");
  // memory guard
  result = mprotect(ptr,getpagesize(), PROT_NONE);
  assert(result == 0 && "Couldn't set page protection");

  result = mprotect((char*)ptr + getpagesize() + m_stack_size, getpagesize(), PROT_NONE);
  assert(result == 0 && "Couldn't set page protection");
  exec_context->p_stack = (char*)ptr;
}

void
execution_context_release(execution_context_t* exec_context)
{
  int result = mprotect(exec_context->p_stack, getpagesize(), PROT_READ | PROT_WRITE);
  assert(result == 0 && "Couldn't remove page protection");
  result = mprotect(exec_context->p_stack + getpagesize() + m_stack_size, getpagesize(), PROT_READ | PROT_WRITE);
  assert(result == 0 && "Couldn't remove page protection");
  free(exec_context->p_stack);
}

void
execution_context_reset(execution_context_t* exec_context)
{
  exec_context->m_context = nullptr;
}

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

/**
 * @brief  Structure used to represent a task to be executed by the thread pool
 */
struct task_context_t {

  /**
   * @brief Task encapsulated within this task context
   */
  task_t                m_task;

  /**
   * @brief Synchronization counter used to synchronize this task
   */
  atomic_counter_t*     p_syn_counter = nullptr;

  /**
   * @bried The execution context of this task
   */
  execution_context_t*  p_context     = nullptr;

  /**
   * @brief Whether the task is finished or not
   */
  bool                  m_finished    = false;

  /**
   * @brief A pointer to the parent of the task in the task dependency graph
   */
  task_context_t*      p_parent       = nullptr;

  /**
   * \brief Task name
   */
  char                      m_name[_TNA_TASKING_MAX_NAME_STRING_LENGTH];

  /**
   * @brief Task info
   */
  char               m_info[_TNA_TASKING_MAX_INFO_STRING_LENGTH];


};  

void
task_context_reset(task_context_t* task_context)
{
  task_context->m_task.p_fp   = nullptr;
  task_context->m_task.p_args = nullptr;
  task_context->p_syn_counter = nullptr;
  task_context->p_context     = nullptr;
  task_context->m_finished    = false;
  task_context->p_parent      = nullptr;
  task_context->m_info[0]     = '\0'; 
}

/**
 * \brief Queue with available task contexts
 */
static execution_context_queue_t*        p_execution_context_queue = nullptr;

/**
 * \brief Mutex to synchronize access to task_context_queue
 */
static mutex_t                        m_execution_context_queue_mutex;

/**
 * \brief Queue with available task contexts
 */
static task_context_queue_t*        p_task_context_queue = nullptr;

/**
 * \brief Mutex to synchronize access to task_context_queue
 */
static mutex_t                          m_task_context_queue_mutex;

/**
 * \brief stores if the threadpool is initialized
 */
static bool                         m_initialized = false;

/**
 * @brief Atomic booleans to control threads running
 */
static std::atomic<bool>*           m_is_running = nullptr;

/**
 * @brief The number of threads in the thread pool
 */
static uint32_t                     m_num_threads = 0;

/**
 * @brief Vector of running threads objects
 */
static std::thread**                p_threads = nullptr;

/**
 * @brief Pointer to the task pool with the tasks pending to start 
 */
static task_pool_t                    m_to_start_task_pool;

/**
 * @brief Vectors holding the running tasks of the thread.
 */
static task_pool_t                   m_running_task_pool;

/**
 * @brief Array of Contexts used to store the main context of each thread to
 * fall back to it when yielding a task
 */
static execution_context_t*            m_worker_contexts = nullptr;

/**
 * @brief Mutexes used for the condition variables for notifying sleeping
 * threads that more work is ready
 */
static std::mutex*                  m_condvar_mutexes = nullptr;

/**
 * @brief Condition variables sed to notify sleeping threads that more work is
 * ready
 */
static std::condition_variable*     m_condvars = nullptr;
/**
 * @brief Boolean flag used for condition variables for notifying sleepting
 * threads that more work is ready
 */
static bool*                        m_ready = nullptr;

/**
 * \brief Timing event arrays for each of the threads
 */
static task_timing_event_t**  p_timing_event_arrays = nullptr;

/**
 * \brief The timing event arrays count
 */
static uint32_t*              m_timing_event_count = nullptr;

/**
 * \brief The timing event arrays capacity
 */
static uint32_t*              m_timing_event_capacity = nullptr;

/**
 * \brief Mutex for event arrays
 */
static mutex_t               m_timing_event_mutex;


/**
 * \brief To record timing events or not
 */
static bool                          m_record_timings = false;

/**
 * @brief Pointer to the thread local currently running task 
 */
static thread_local task_context_t*    p_current_running_task = nullptr;

/**
 * @brief Thread local variable to store the id of the current thread
 */
static thread_local uint32_t        m_current_thread_id = INVALID_THREAD_ID;


/**
 * \brief Inserts a timing event into the correponding queue
 *
 * \param queueid The queueid
 * \param event_type The event_type
 */
static void
insert_timing_event(uint32_t queue_id, 
                    task_timing_event_type_t event_type, 
                    task_context_t* task_context)
{
#ifdef _TNA_DEV
  mutex_lock(&m_timing_event_mutex);
  if(m_record_timings)
  {
    task_timing_event_t timing_event;

    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    timing_event.m_time_ns = time.tv_sec * 1000 * 1000 * 1000 + time.tv_nsec;
    timing_event.m_event_type = event_type;
    if(task_context != nullptr)
    {
      strncpy(timing_event.m_info, task_context->m_info, _TNA_TASKING_MAX_INFO_STRING_LENGTH);
      strncpy(timing_event.m_name, task_context->m_name, _TNA_TASKING_MAX_NAME_STRING_LENGTH);
    }
    else
    {
      timing_event.m_info[0] = '\0';
    }
    if(m_timing_event_count[queue_id] >= m_timing_event_capacity[queue_id])
    {
      uint32_t new_capacity = m_timing_event_capacity[queue_id]*2;
      task_timing_event_t* new_buffer = new task_timing_event_t[new_capacity];
      memcpy(new_buffer, p_timing_event_arrays[queue_id], sizeof(task_timing_event_t)*m_timing_event_count[queue_id]);
      m_timing_event_capacity[queue_id] = new_capacity;
      delete [] p_timing_event_arrays[queue_id];
      p_timing_event_arrays[queue_id] = new_buffer;
    }

    (p_timing_event_arrays[queue_id])[m_timing_event_count[queue_id]] = timing_event;
    m_timing_event_count[queue_id]++;
  }
  mutex_unlock(&m_timing_event_mutex);
#endif
}

/**
 * @brief Finalizes the current running task and releases its resources
 */
static void 
finalize_current_running_task() {
  assert(tasking_get_current_thread_id() < m_num_threads && "Invalid thead id");

  if(p_current_running_task->m_finished) 
  {

    // insert event
    insert_timing_event(tasking_get_current_thread_id(), 
                        task_timing_event_type_t::E_STOP, 
                        p_current_running_task);

    if(p_current_running_task->p_syn_counter != nullptr) 
    {
      atomic_counter_fetch_decrement(p_current_running_task->p_syn_counter);
    }
    mutex_lock(&m_execution_context_queue_mutex);
    queue_push(p_execution_context_queue, p_current_running_task->p_context);
    mutex_unlock(&m_execution_context_queue_mutex);
    p_current_running_task->p_context = nullptr;

    mutex_lock(&m_task_context_queue_mutex);
    queue_push(p_task_context_queue, p_current_running_task);
    mutex_unlock(&m_task_context_queue_mutex);
  }
  else 
  {
    task_pool_add_task(&m_running_task_pool, 
                       m_current_thread_id, 
                       p_current_running_task);
  }
  p_current_running_task = nullptr;
}

void
fiber_function(void* arg)
{
  assert(tasking_get_current_thread_id() < m_num_threads && "Invalid thead id");
  
  task_context_t* task_context = (task_context_t*)arg;
  task_context->m_task.p_fp(task_context->m_task.p_args);
  task_context->m_finished = true;
  boost_context::jump_fcontext(&task_context->p_context->m_context, 
                               m_worker_contexts[tasking_get_current_thread_id()].m_context, 
                               task_context->m_task.p_args);

}

/**
 * @brief Start the execution of the given task. A lambda function is passed
 * to the callcc method, which takes as input the current execution context.
 * This current execution context is stored int he m_worker_contexts array
 * for the current thread. Then the task is executed. 
 *
 * @param task The task to execute
 */
static void 
start_task(task_context_t* task_context) 
{
  assert(tasking_get_current_thread_id() < m_num_threads && "Invalid thead id");

  // find free fiber
  mutex_lock(&m_execution_context_queue_mutex);
  execution_context_t* ctx = nullptr; 
  queue_pop(p_execution_context_queue, &ctx);
  mutex_unlock(&m_execution_context_queue_mutex);

  if(ctx == nullptr)
  {
    ctx = new execution_context_t();
    execution_context_init(ctx);
  }
  else
  {
    // we reset it in case it is a reused context
    execution_context_reset(ctx);
  }

  // initialize fiber task function
  task_context->p_context = ctx;

  ctx->m_context = boost_context::make_fcontext(ctx->p_stack + getpagesize() + m_stack_size, 
                                                m_stack_size, 
                                                fiber_function);
  // set the task as the current running task
  p_current_running_task = task_context; 

  // insert event
  insert_timing_event(tasking_get_current_thread_id(), 
                      task_timing_event_type_t::E_START, 
                      p_current_running_task);

  // jump to fiber
  boost_context::jump_fcontext(&(m_worker_contexts[tasking_get_current_thread_id()].m_context), 
                               ctx->m_context, 
                               task_context);
  finalize_current_running_task();
}


/**
 * @brief Resumes the given running task 
 *
 * @param runningTask The task to resume
 */
static void 
resume_task(task_context_t* runningTask) 
{
  assert(tasking_get_current_thread_id() < m_num_threads && "Invalid thead id");

  p_current_running_task = runningTask;

  // insert event
  insert_timing_event(tasking_get_current_thread_id(), 
                      task_timing_event_type_t::E_RESUME, 
                      p_current_running_task);

  // call jump to fiber to resume
  boost_context::jump_fcontext(&(m_worker_contexts[tasking_get_current_thread_id()].m_context), 
                               runningTask->p_context->m_context, 
                               runningTask);
  finalize_current_running_task();
}

/**
 * @brief The main function that each thread runs
 *
 * @param threadId The id of the thread that runs the function
 */
static void 
thread_function(int threadId) 
{
  m_current_thread_id = threadId;
  while(m_is_running[m_current_thread_id].load()) 
  {
    task_context_t* task_context = task_pool_get_next(&m_to_start_task_pool, m_current_thread_id);
    if(task_context != nullptr) 
    {
      start_task(task_context);
    } 
    else if ( (task_context = task_pool_get_next(&m_running_task_pool, m_current_thread_id)) != nullptr)
    {
      resume_task(task_context);
    } 
    else 
    {
      // Wait until notified
      std::unique_lock<std::mutex> lock(m_condvar_mutexes[m_current_thread_id]);
      m_ready[m_current_thread_id] = false;
      m_condvars[m_current_thread_id].wait_for(lock,
                                             std::chrono::microseconds(100), 
                                             [] { return m_ready[m_current_thread_id]; });
                                             
                                             
    }
  }
}

void 
tasking_start_thread_pool(uint32_t numThreads) 
{

  m_num_threads          = numThreads;

  if(m_num_threads > 0) 
  {

    // computing stack_size
    constexpr size_t _TNA_STACK_SIZE = 16384;
    size_t page_size = getpagesize();
    size_t reminder = _TNA_STACK_SIZE % page_size;
    if( reminder == 0)
    {
      m_stack_size = _TNA_STACK_SIZE;
    }
    else
    {
      m_stack_size = _TNA_STACK_SIZE + page_size - reminder;
    }


    // initializing resources
    task_pool_init(&m_to_start_task_pool, numThreads);
    task_pool_init(&m_running_task_pool, numThreads);
    m_worker_contexts  = new execution_context_t[numThreads];
    for(uint32_t i = 0; i < m_num_threads; ++i)
    {
      execution_context_init(&m_worker_contexts[i]);
    }

    m_is_running          = new std::atomic<bool>[m_num_threads];
    p_threads             = new std::thread*[m_num_threads];
    m_condvar_mutexes     = new std::mutex[m_num_threads];
    m_condvars            = new std::condition_variable[m_num_threads];
    m_ready               = new bool[m_num_threads];
    p_timing_event_arrays = new task_timing_event_t*[m_num_threads+1];
    m_timing_event_count  = new uint32_t[m_num_threads+1];
    m_timing_event_capacity = new uint32_t[m_num_threads+1];
    p_execution_context_queue   = new execution_context_queue_t();
    queue_init(p_execution_context_queue, 4096);
    mutex_init(&m_execution_context_queue_mutex);
    mutex_init(&m_timing_event_mutex);

    p_task_context_queue   = new task_context_queue_t();
    queue_init(p_task_context_queue, 4096);
    mutex_init(&m_task_context_queue_mutex);


    for(uint32_t i = 0; i < m_num_threads; ++i) {
      m_is_running[i].store(true);
      m_ready[i] = false;
      m_timing_event_count[i] = 0;
      m_timing_event_capacity[i] = 1024;
      p_timing_event_arrays[i] = new task_timing_event_t[1024];
      p_threads[i] = new std::thread(thread_function, i);
      cpu_set_t cpuset;
      CPU_ZERO(&cpuset);
      CPU_SET(i, &cpuset);
      pthread_setaffinity_np(p_threads[i]->native_handle(),
                             sizeof(cpu_set_t), &cpuset);
    }

    p_timing_event_arrays[m_num_threads] = new task_timing_event_t[1024];
    m_timing_event_count[m_num_threads] = 0;
    m_timing_event_capacity[m_num_threads] = 1024;

    m_initialized = true;

  }

}

void 
tasking_stop_thread_pool() 
{
  assert(m_initialized && "ThreadPool is not initialized");

  if(m_num_threads > 0) {

    // Telling threads to stop
    for(uint32_t i = 0; i < m_num_threads; ++i) {
      m_is_running[i].store(false);
    }

    // Notify threads to continue
    for(uint32_t i = 0; i < m_num_threads; ++i) {
      {
        std::lock_guard<std::mutex> guard(m_condvar_mutexes[i]);
        m_ready[i] = true;
      }
      m_condvars[i].notify_all();
    }

    // Waitning threads to stop
    for(uint32_t i = 0; i < m_num_threads; ++i) {
      p_threads[i]->join();
      delete p_threads[i];
    }

    for (uint32_t i = 0; i < m_num_threads; ++i) 
    {
      delete [] p_timing_event_arrays[i];
    }
    delete [] p_timing_event_arrays[m_num_threads];

    delete [] p_timing_event_arrays;
    delete [] m_timing_event_capacity;
    delete [] m_timing_event_count;
    mutex_release(&m_timing_event_mutex);

    delete [] m_ready;
    delete [] m_condvars;
    delete [] m_condvar_mutexes;
    delete [] p_threads;
    delete [] m_is_running;

    for (uint32_t i = 0; i < m_num_threads; ++i) 
    {
      execution_context_release(&m_worker_contexts[i]);
    }
    delete [] m_worker_contexts;

    task_pool_release(&m_running_task_pool);
    task_pool_release(&m_to_start_task_pool);

    execution_context_t* execution_context = nullptr;
    while(queue_pop(p_execution_context_queue, &execution_context))
    {
      execution_context_release(execution_context);
      delete execution_context;
    }
    queue_release(p_execution_context_queue);
    delete p_execution_context_queue;
    mutex_release(&m_execution_context_queue_mutex);

    task_context_t* task_context = nullptr;
    while(queue_pop(p_task_context_queue, &task_context))
    {
      delete task_context;
    }
    queue_release(p_task_context_queue);
    delete p_task_context_queue;
    mutex_release(&m_task_context_queue_mutex);
  }
  m_initialized = false;
}

void 
tasking_execute_task_async(uint32_t queueId, 
                   task_t task, 
                   atomic_counter_t* counter,
                   const char* name, 
                   const char* info) 
{
  assert(m_initialized && "ThreadPool is not initialized");
  assert(m_num_threads > 0 && "Number of threads in the threadpool must be > 0");
  
  mutex_lock(&m_task_context_queue_mutex);
  task_context_t* task_context = nullptr;
  queue_pop(p_task_context_queue, &task_context);
  mutex_unlock(&m_task_context_queue_mutex);

  if(task_context == nullptr)
  {
    task_context = new task_context_t();
  }
  else
  { 
    // we reset it in case it is a reused context
    task_context_reset(task_context);
  }

  task_context->m_task = task;
  task_context->p_syn_counter = counter;
  task_context->m_name[0] = '\0';
  if(name != nullptr)
  {
    strncpy(task_context->m_name, name, _TNA_TASKING_MAX_NAME_STRING_LENGTH-1);
    if(strlen(info) >= _TNA_TASKING_MAX_NAME_STRING_LENGTH)
    {
      task_context->m_name[_TNA_TASKING_MAX_NAME_STRING_LENGTH-1] = '\0';
    }
  }
  task_context->m_info[0] = '\0';
  if(info != nullptr)
  {
    strncpy(task_context->m_info, info, _TNA_TASKING_MAX_INFO_STRING_LENGTH);
    if(strlen(info) >= _TNA_TASKING_MAX_INFO_STRING_LENGTH)
    {
      task_context->m_info[_TNA_TASKING_MAX_INFO_STRING_LENGTH-1] = '\0';
    }
  }

  if(task_context->p_syn_counter != nullptr) 
  {
    atomic_counter_fetch_increment(task_context->p_syn_counter);
  }

  if(tasking_get_current_thread_id() != INVALID_THREAD_ID ) 
  {
    task_context->p_parent = p_current_running_task;
  } 
  else 
  {
    task_context->p_parent = nullptr;
  }

  task_pool_add_task(&m_to_start_task_pool,
                     queueId, 
                     task_context);

  {
    std::lock_guard<std::mutex> guard(m_condvar_mutexes[queueId]);
    m_ready[queueId] = true;
  }
  m_condvars[queueId].notify_all();
} 

void 
tasking_execute_task_sync(uint32_t queueId, 
                  task_t task, 
                  atomic_counter_t* counter, 
                  const char* name, 
                  const char* info) 
{
  assert(m_initialized && "ThreadPool is not initialized");
  assert(m_num_threads > 0 && "Number of threads in the threadpool must be > 0");
  tasking_execute_task_async(queueId, task, counter, name, info);
  atomic_counter_join(counter);
}

uint32_t 
tasking_get_current_thread_id() 
{
  assert(((m_current_thread_id >= 0 && m_current_thread_id < m_num_threads) || m_current_thread_id == INVALID_THREAD_ID) && "Invalid thread id");
  return m_current_thread_id;
}


void 
tasking_yield() 
{
  assert(m_initialized && "ThreadPool is not initialized");
  assert(m_num_threads > 0 && "Number of threads in the threadpool must be > 0");
  assert(m_current_thread_id != INVALID_THREAD_ID);

  insert_timing_event(tasking_get_current_thread_id(), 
                      task_timing_event_type_t::E_YIELD, 
                      p_current_running_task);

  boost_context::jump_fcontext(&(p_current_running_task->p_context->m_context), m_worker_contexts[m_current_thread_id].m_context, nullptr);
}

uint32_t
tasking_get_num_threads() 
{
  assert(m_initialized && "ThreadPool is not initialized");
  return m_num_threads;
}

void
tasking_start_recording_timings()
{
  mutex_lock(&m_timing_event_mutex);
  m_record_timings = true;
  for(uint32_t i = 0; i < m_num_threads; ++i)
  {
    m_timing_event_count[i] = 0;
  }
  mutex_unlock(&m_timing_event_mutex);
}

void
tasking_stop_recording_timings()
{
  mutex_lock(&m_timing_event_mutex);
  m_record_timings = false;
  mutex_unlock(&m_timing_event_mutex);
}

void
tasking_record_new_frame()
{
  insert_timing_event(m_num_threads, 
                      task_timing_event_type_t::E_NEW_FRAME, 
                      nullptr);
}

task_timing_event_t*
tasking_get_task_timing_event_array(uint32_t queue_id, uint32_t* count)
{
  *count = m_timing_event_count[queue_id];
  return p_timing_event_arrays[queue_id];
}

} /* tna */ 
