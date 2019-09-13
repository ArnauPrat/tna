
#include "queue.h"

#include <string.h>
#include <stdlib.h>

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
  queue->p_elements = (T*)malloc(sizeof(T)*queue->m_capacity);

#ifndef NDEBUG
  memset(queue->p_elements, 0, sizeof(T)*queue->m_capacity);
#endif

}

template <typename T>
void
queue_release(queue_t<T>* queue)
{
  if(queue->p_elements != nullptr)
  {
    free(queue->p_elements);
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
bool
queue_pop(queue_t<T>* queue, T* element)
{
  if(queue->m_num_elements == 0)
  {
    return false;
  }

  *element = queue->p_elements[queue->m_start];

#ifndef NDEBUG
  memset(&queue->p_elements[queue->m_start], 0, sizeof(T));
#endif

  queue->m_start++;
  if(queue->m_start == queue->m_capacity)
  {
    queue->m_start = 0;
  }
  queue->m_num_elements--;
  return true;
}

template <typename T>
void
queue_resize(queue_t<T>* queue)
{
  uint32_t new_capacity = queue->m_capacity*2;
  T* buffer = (T*)malloc(sizeof(T)*new_capacity);

#ifndef NDEBUG
  memset(buffer, 0, sizeof(T)*new_capacity);
#endif

  // we copy from m_start to the end of the array to the beginning of the buffer
  memcpy(buffer, 
         &queue->p_elements[queue->m_start], 
         sizeof(T)*(queue->m_capacity - queue->m_start));

  if(queue->m_start >= queue->m_end)
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
  free(queue->p_elements);
  queue->p_elements = buffer;
}
  
} /*  tna */ 
