

#ifndef _TNA_DATA_QUEUE_H_
#define _TNA_DATA_QUEUE_H_ value

#include "../types.h"

namespace tna
{

template <typename T>
struct queue_t
{
  uint32_t  m_start         = 0;
  uint32_t  m_end           = 0; 
  uint32_t  m_num_elements  = 0;
  uint32_t  m_capacity      = 0;
  T*        p_elements      = nullptr;
};

template <typename T>
void
queue_init(queue_t<T>* queue, 
           uint32_t initial_capacity);

template <typename T>
void
queue_release(queue_t<T>* queue);

template <typename T>
void
queue_push(queue_t<T>* queue, 
           T element);


template <typename T>
T
queue_pop(queue_t<T>* queue);

} /* tna */ 

#include "queue.inl"

#endif /* ifndef _TNA_TASKING_TASK_QUEUE_H_ */
