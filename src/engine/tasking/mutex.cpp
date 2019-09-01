
#include "mutex.h"
#include <mutex>

namespace tna 
{

void
mutex_init(mutex_t* mutex)
{
  mutex->p_mutex_impl = new std::mutex();
}

void
mutex_release(mutex_t* mutex)
{
  delete mutex->p_mutex_impl;
}

void
mutex_lock(mutex_t*  mutex)
{
  mutex->p_mutex_impl->lock();
}

void
mutex_unlock(mutex_t* mutex)
{
  mutex->p_mutex_impl->unlock();
}
  
} /* mutex */ 
