

#ifndef _TNA_TASKING_THREAD_POOL_H_
#define _TNA_TASKING_THREAD_POOL_H_

#include "../types.h"
#include "task.h"

namespace tna
{

struct atomic_counter_t;
  
#define INVALID_THREAD_ID 0xffffffff

/**
 * @brief Initializes and starts the thread pool, and creates the task queues. 
 * Currently, each thred has its own queue id, which equals the id of the thread. 
 */
void start_thread_pool(uint32_t num_threads);

/**
 * @brief Stops the thread pool
 */
void stop_thread_pool();

/**
 * @brief Sends the given task for execution at the given thread
 *
 * @param task The task to run
 * @param queueId The queueId to add the task to
 * @param counter A pointer to the atomic_counter_t that will be used for
 * synchronization
 */
void execute_task_async(uint32_t queueId, 
                        task_t task,
                        atomic_counter_t* counter);

/**
 * @brief Sends the given task for execution at the given thread, and blocks
 * until it finishes
 *
 * @param task The task to run
 * @param queueId The queueId to add the task to
 * @param counter A pointer to the atomic_counter_t that will be used for
 * synchronization
 */
void execute_task_sync(uint32_t queueId, 
                       task_t task, 
                       atomic_counter_t* counter);

/**
 * @brief Gets the current thread id
 *
 * @return  The id of the thread currently running
 */
uint32_t 
get_current_thread_id();

/**
 * @brief Yields the current task and returns execution path to the thread pool
 * to pick another task
 */
void yield();

/**
 * @brief Gets the number of threads available in the buffer pool
 *
 * @return Returns the number of available threads
 */
uint32_t get_num_threads();

} /* tna */ 

#endif /* ifndef _TASKING_THREAD_POOL_H_ */
