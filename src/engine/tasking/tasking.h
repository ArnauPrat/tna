

#ifndef _TNA_TASKING_THREAD_POOL_H_
#define _TNA_TASKING_THREAD_POOL_H_

#include "../types.h"
#include "task.h"
#include <assert.h>
#include <chrono>

namespace tna
{

struct atomic_counter_t;

#define INVALID_THREAD_ID 0xffffffff
#define _TNA_TASKING_MAX_INFO_STRING_LENGTH 1024
#define _TNA_TASKING_MAX_NAME_STRING_LENGTH 128

using yield_func_t = bool (*)(void*);

template<typename T>
struct
queue_t;


enum class task_timing_event_type_t
{
  E_START = 0,
  E_RESUME,
  E_YIELD,
  E_STOP,
  E_NEW_FRAME,
  E_UNKNOWN
};

struct task_timing_event_t
{
  uint64_t                  m_time_ns = 0;
  task_timing_event_type_t  m_event_type = task_timing_event_type_t::E_UNKNOWN;
  char                      m_name[_TNA_TASKING_MAX_NAME_STRING_LENGTH] = {'\0'};
  char                      m_info[_TNA_TASKING_MAX_INFO_STRING_LENGTH] = {'\0'};
};

using task_timing_event_queue_t = queue_t<task_timing_event_t>;

/**
 * \brief Initializes and starts the thread pool, and creates the task queues. 
 * Currently, each thred has its own queue id, which equals the id of the thread. 
 */
void 
tasking_start_thread_pool(uint32_t num_threads);

/**
 * \brief Stops the thread pool
 */
void 
tasking_stop_thread_pool();

/**
 * \brief Sends the given task for execution at the given thread
 *
 * \param task The task to run
 * \param queueId The queueId to add the task to
 * \param counter A pointer to the atomic_counter_t that will be used for
 * synchronization
 * \param info String unsed for debugging/dev 
 */
void 
tasking_execute_task_async(uint32_t queueId, 
                           task_t task,
                           atomic_counter_t* counter,
                           const char* name,
                           const char* info);

/**
 * \brief Sends the given task for execution at the given thread, and blocks
 * until it finishes
 *
 * \param task The task to run
 * \param queueId The queueId to add the task to
 * \param counter A pointer to the atomic_counter_t that will be used for
 * synchronization
 * \param info String unsed for debugging/dev 
 */
void 
tasking_execute_task_sync(uint32_t queueId, 
                          task_t task, 
                          atomic_counter_t* counter, 
                          const char* name,
                          const char* info);

/**
 * \brief Gets the current thread id
 *
 * \return  The id of the thread currently running
 */
uint32_t 
tasking_get_current_thread_id();

/**
 * \brief Yields the current task and returns execution path to the thread pool
 * to pick another task
 */
void 
tasking_yield();

void
tasking_yield(yield_func_t yield_func, void* data);

void
tasking_yield(atomic_counter_t* sync_counter);

/**
 * @brief Gets the number of threads available in the buffer pool
 *
 * @return Returns the number of available threads
 */
uint32_t
tasking_get_num_threads();

/**
 * \brief Starts recording the timings of executed tasks
 */
void
tasking_start_recording_timings();

/**
 * \brief Stops recording the timings of executed tasks
 */
void
tasking_stop_recording_timings();

/**
 * \brief Records a new frame timing event
 */
void
tasking_record_new_frame();

/**
 * \brief Gets the event queue for the specified queue
 *
 * \param queueid The queue id to get
 * \param count Pointer to variable to store the count in the array
 *
 * \return Returns the pointer to the queue
 */
task_timing_event_t*
tasking_get_task_timing_event_array(uint32_t queueid, uint32_t* count);

} /* tna */ 

#endif /* ifndef _TASKING_THREAD_POOL_H_ */
