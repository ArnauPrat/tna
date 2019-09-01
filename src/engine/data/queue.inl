
#include "queue.h"

#include <string.h>

namespace tna
{

template <typename T>
void
queue_init(queue_t<T>* queue, 
           uint32_t initial_capacity)
{
  queue->m_num_elements = 0;
  queue->m_capacity = initial_capacity;
  queue->m_start = 0;
  queue->m_end = 0;
  queue->p_elements = new T[initial_capacity];
}

template <typename T>
void
queue_release(queue_t<T>* queue)
{
  if(queue->p_elements != nullptr)
  {
    delete [] queue->p_elements;
    queue->m_num_elements = 0;
    queue->m_capacity = 0;
  }
}

template <typename T>
void
queue_push(queue_t<T>* queue, 
           T element)
{
  if(queue->m_capacity == queue->m_num_elements)
  {
    queue_resize(queue);
  }

  queue->p_elements[queue->m_end] = element;
  queue->m_end++;
  if(queue->m_end == queue->m_capacity)
  {
    queue->m_end = 0;
  }
  queue->m_num_elements++;
}

template <typename T>
T
queue_pop(queue_t<T>* queue)
{
  if(queue->m_num_elements == 0)
  {
    return nullptr;
  }

  T* ret = &queue->p_elements[queue->m_start];
  queue->m_start++;
  if(queue->m_start == queue->m_capacity)
  {
    queue->m_start = 0;
  }
  return *ret;
}

template <typename T>
void
queue_resize(queue_t<T>* queue)
{
  uint32_t new_capacity = queue->m_capacity*2;
  T* buffer = new T[new_capacity];

  // we copy from m_start to the end of the array to the beginning of the buffer
  memcpy(buffer, 
         &queue->p_elements[queue->m_start], 
         sizeof(T)*(queue->m_capacity - queue->m_start));

  if(queue->m_start > queue->m_end)
  {
    // we copy from the beginning of the array to m_end to the remaining of the
    // buffer
    memcpy(&buffer[queue->m_capacity - queue->m_start], 
           queue->p_elements, 
           sizeof(T)*(queue->m_end));
  }

  queue->m_start = 0;
  queue->m_end = queue->m_num_elements; 
  queue->m_capacity = new_capacity;
  delete [] queue->p_elements;
  queue->p_elements = buffer;
}
  
} /*  tna */ 
