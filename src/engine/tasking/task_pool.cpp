
#include "task_pool.h"
#include "../data/queue.h"
#include <assert.h>

namespace  tna
{
  
void
task_pool_init(task_pool_t* task_pool,
               uint32_t num_qeues)
{
  task_pool->m_num_queues = num_qeues;
  task_pool->m_queues = new task_context_queue_t[task_pool->m_num_queues];
  for(uint32_t i = 0; i < task_pool->m_num_queues; ++i) 
  {
    queue_init(&task_pool->m_queues[i], 4096);
  }

  task_pool->m_mutexes = new mutex_t[task_pool->m_num_queues];

  for(uint32_t i = 0; i < task_pool->m_num_queues; ++i) 
  {
    mutex_init(&task_pool->m_mutexes[i]);
  }
}

void
task_pool_release(task_pool_t* task_pool) 
{
  if(task_pool->m_queues)
  {
    for(uint32_t i = 0; i < task_pool->m_num_queues; ++i) 
    {
      queue_release(&task_pool->m_queues[i]);
    }
    delete [] task_pool->m_queues;
    task_pool->m_queues = nullptr;
  }

  if(task_pool->m_mutexes)
  {
    for(uint32_t i = 0; i < task_pool->m_num_queues; ++i) 
    {
      mutex_release(&task_pool->m_mutexes[i]);
    }
    delete [] task_pool->m_mutexes;
    task_pool->m_mutexes = nullptr;
  }
}

task_context_t* 
task_pool_get_next(task_pool_t* task_pool,
                   uint32_t queueId) 
{
  assert(queueId < task_pool->m_num_queues && queueId >= 0 && "Invalid thread queue id");
  mutex_lock(&task_pool->m_mutexes[queueId]);
  task_context_t* task = nullptr;
  if(!queue_pop(&task_pool->m_queues[queueId], &task))
  {
    task = nullptr;
  }
  mutex_unlock(&task_pool->m_mutexes[queueId]);
  return task;
}

uint32_t 
task_pool_count(task_pool_t* task_pool,
                   uint32_t queueId) 
{
  assert(queueId < task_pool->m_num_queues && queueId >= 0 && "Invalid thread queue id");
  mutex_lock(&task_pool->m_mutexes[queueId]);
  uint32_t count = queue_count(&task_pool->m_queues[queueId]);
  mutex_unlock(&task_pool->m_mutexes[queueId]);
  return count;
}

void 
task_pool_add_task(task_pool_t* task_pool,
                   uint32_t queueId, 
                   task_context_t* task) 
{
  assert(queueId < task_pool->m_num_queues && queueId >= 0 && "Invalid thread queue id");
  mutex_lock(&task_pool->m_mutexes[queueId]);
  queue_push(&task_pool->m_queues[queueId], task);
  mutex_unlock(&task_pool->m_mutexes[queueId]);
}

}

