
#ifndef _TASKING_TASK_POOL_H_
#define _TASKING_TASK_POOL_H_ value

#include "../types.h"
#include "mutex.h"
#include "task.h"

namespace tna
{
  

struct task_context_t;
template <typename T>
struct queue_t;

using task_context_queue_t = queue_t<task_context_t*>;

/**
 * @brief Class use to store the different tasks that threads in the thread pool
 * can consume and process. The task pool has different independent queues.
 * Typically, there will be one queue per thread, but we are not restricted to
 * this.
 */
struct task_pool_t {
  /**
   * @brief Array of task queues. There is one per thread.
   */
  task_context_queue_t*   m_queues;

  /**
   * \brief Mutexes used to synchronize the access to the queues
   */
  mutex_t*                m_mutexes;    

  /**
   * @brief The number of queues
   */
  uint32_t             m_numQueues;

};

/**
 * \brief Initialize the task pool
 *
 * \param task_pool The task pool to initialize
 */
void
task_pool_init(task_pool_t* task_pool,
               uint32_t num_queues);

/**
 * \brief Releases the task pool
 *
 * \param task_pool The task pool to release
 */
void
task_pool_release(task_pool_t* task_pool);


/**
 * @brief Adds a task to execute in the given queue
 *
 * @param queueId
 * @param task_context
 */
void
task_pool_add_task(task_pool_t* task_pool,
                   uint32_t queue_id,
                   task_context_t* task_context);


/**
 * @brief Gets the next task to process
 *
 * @param queueId The thread to get the task for 
 *
 * @return Pointer to the task if this exists. nullptr otherwise. 
 */
task_context_t*
task_pool_get_next(task_pool_t* task_pool,
                   uint32_t queue_id);

}

#endif /* ifndef _TASKING_TASK_POOL_H_ */
