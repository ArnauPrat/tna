
#include "../data/queue.h"
#include "../types.h"
#include "atomic_counter.h"
#include "mutex.h"
#include "task_pool.h"
#include "tasking.h"
#include <boost_context/fcontext.h>

#include <assert.h>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>



#define _TNA_TASKING_STACK_SIZE 16384

namespace tna
{

struct execution_context_t;

using execution_context_queue_t = queue_t<execution_context_t*>;

/**
 * \brief Stores the context of an execution
 */
struct execution_context_t
{
  char* p_stack;
  boost_context::fcontext_t m_context;
};

void
execution_context_init(execution_context_t* exec_context)
{
  exec_context->p_stack = new char[_TNA_TASKING_STACK_SIZE];
}

void
execution_context_release(execution_context_t* exec_context)
{
  delete [] exec_context->p_stack;
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
  atomic_counter_t*     p_syn_counter;

  /**
   * @bried The execution context of this task
   */
  execution_context_t*  p_context;

  /**
   * @brief Whether the task is finished or not
   */
  bool                  m_finished;

  /**
   * @brief A pointer to the parent of the task in the task dependency graph
   */
  task_context_t*      p_parent;

};  

void
task_context_init(task_context_t* context)
{
  context->m_finished = false;
  context->p_parent = nullptr;
  context->p_syn_counter = nullptr;
  context->p_context = nullptr;
}

void
task_context_release(task_context_t* context)
{
  context->m_finished = false;
  context->p_parent = nullptr;
  context->p_syn_counter = nullptr;
  context->p_context = nullptr;
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

static bool                         m_initialized = false;

/**
 * @brief Atomic booleans to control threads running
 */
static std::atomic<bool>*           m_is_running = nullptr;

/**
 * @brief The number of threads in the thread pool
 */
static uint32_t                  m_num_threads = 0;

/**
 * @brief Vector of running threads objects
 */
static std::thread**                p_threads;

/**
 * @brief Pointer to the task pool with the tasks pending to start 
 */
static task_pool_t                    m_to_start_task_pool;

/**
 * @brief Vectors holding the running tasks of the thread.
 */
static task_pool_t                   m_running_task_pool;


/**
 * @brief Thread local variable to store the id of the current thread
 */
static thread_local uint32_t        m_current_thread_id = INVALID_THREAD_ID;

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
 * @brief Pointer to the thread local currently running task 
 */
static thread_local task_context_t*    p_current_running_task = nullptr;

/**
 * @brief Finalizes the current running task and releases its resources
 */
static void 
finalize_current_running_task() {
  if(p_current_running_task->m_finished) 
  {
    if(p_current_running_task->p_syn_counter != nullptr) 
    {
      atomic_counter_fetch_decrement(p_current_running_task->p_syn_counter);
    }
    queue_push(p_execution_context_queue, p_current_running_task->p_context);
    p_current_running_task->p_context = nullptr;
    queue_push(p_task_context_queue, p_current_running_task);
  }
  else 
  {
    task_pool_add_task(&m_running_task_pool, 
                       m_current_thread_id, 
                       p_current_running_task);
  }
  p_current_running_task = nullptr;
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
  p_current_running_task = task_context; // TODO: A pool should be used here

  // find free fiber
  mutex_lock(&m_execution_context_queue_mutex);
  execution_context_t* ctx = queue_pop(p_execution_context_queue);
  mutex_unlock(&m_execution_context_queue_mutex);

  if(ctx == nullptr)
  {
    ctx = new execution_context_t();
    execution_context_init(ctx);
  }

  // initialize fiber task function
  task_context->p_context = ctx;
  ctx->m_context = boost_context::make_fcontext(ctx->p_stack + _TNA_TASKING_STACK_SIZE, 
                                                _TNA_TASKING_STACK_SIZE, 
                                                task_context->m_task.p_fp);
  // jump to fiber
  boost_context::jump_fcontext(&m_worker_contexts->m_context, ctx->m_context, task_context->m_task.p_args);
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
  p_current_running_task = runningTask;
  // call jump to fiber to resume
  boost_context::jump_fcontext(&p_current_running_task->p_context->m_context, 
                               runningTask->p_context->m_context, 
                               runningTask->m_task.p_args);
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
    task_context_t* task = task_pool_get_next(&m_to_start_task_pool, m_current_thread_id);
    if(task != nullptr) 
    {
      start_task(task);
    } 
    else if ( (task = task_pool_get_next(&m_running_task_pool, m_current_thread_id)) != nullptr)
    {
      resume_task(task);
    } 
    else 
    {
      // Wait until notified
      std::unique_lock<std::mutex> lock(m_condvar_mutexes[m_current_thread_id]);
      m_ready[m_current_thread_id] = false;
      m_condvars[m_current_thread_id].wait_for(lock,
                                             std::chrono::milliseconds(1), 
                                             [] { return m_ready[m_current_thread_id]; });
    }
  }
}

void 
start_thread_pool(uint32_t numThreads) 
{

  m_num_threads          = numThreads;

  if(m_num_threads > 0) 
  {
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
    p_execution_context_queue   = new execution_context_queue_t();
    queue_init(p_execution_context_queue, 2048);
    mutex_init(&m_execution_context_queue_mutex);
    p_task_context_queue   = new task_context_queue_t();
    queue_init(p_task_context_queue, 2048);
    mutex_init(&m_task_context_queue_mutex);


    for(uint32_t i = 0; i < m_num_threads; ++i) {
      m_is_running[i].store(true);
      p_threads[i] = new std::thread(thread_function, i);
      m_ready[i] = false;
    }
  }
  m_initialized = true;

}

void 
stop_thread_pool() 
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
    while((execution_context = queue_pop(p_execution_context_queue))  != nullptr)
    {
      execution_context_release(execution_context);
      delete [] execution_context;
    }
    queue_release(p_execution_context_queue);
    delete [] p_execution_context_queue;
    mutex_release(&m_execution_context_queue_mutex);

    task_context_t* task_context = nullptr;
    while((task_context = queue_pop(p_task_context_queue))  != nullptr)
    {
      task_context_release(task_context);
      delete [] task_context;
    }
    queue_release(p_task_context_queue);
    delete [] p_task_context_queue;
    mutex_release(&m_task_context_queue_mutex);
  }
  m_initialized = false;
}

void 
execute_task_async(uint32_t queueId, 
                   task_t task, 
                   atomic_counter_t* counter ) 
{
  assert(m_initialized && "ThreadPool is not initialized");
  assert(m_num_threads > 0 && "Number of threads in the threadpool must be > 0");
  
  mutex_lock(&m_task_context_queue_mutex);
  task_context_t* task_context = queue_pop(p_task_context_queue);
  mutex_unlock(&m_task_context_queue_mutex);

  if(task_context == nullptr)
  {
    task_context = new task_context_t();
    task_context_init(task_context);
  }
  task_context->m_task = task;
  task_context->p_syn_counter = counter;
  if(task_context->p_syn_counter != nullptr) 
  {
    atomic_counter_fetch_increment(task_context->p_syn_counter);
  }

  if(get_current_thread_id() != INVALID_THREAD_ID ) 
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
execute_task_sync(uint32_t queueId, 
                     task_t task, 
                     atomic_counter_t* counter) 
{
  assert(m_initialized && "ThreadPool is not initialized");
  assert(m_num_threads > 0 && "Number of threads in the threadpool must be > 0");
  execute_task_async(queueId, task, counter);
  atomic_counter_join(counter);
}

uint32_t get_current_thread_id() 
{
  return m_current_thread_id;
}

void 
yield() 
{
  assert(m_initialized && "ThreadPool is not initialized");
  assert(m_num_threads > 0 && "Number of threads in the threadpool must be > 0");
  assert(m_current_thread_id != INVALID_THREAD_ID);
  boost_context::jump_fcontext(&p_current_running_task->p_context->m_context, m_worker_contexts[m_current_thread_id].m_context, nullptr);
}

uint32_t
get_num_threads() 
{
  assert(m_initialized && "ThreadPool is not initialized");
  return m_num_threads;
}

} /* tna */ 
