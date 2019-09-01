

#ifndef _TNA_TASKING_MUTEX_H_H
#define _TNA_TASKING_MUTEX_H_H value

namespace std
{
  struct mutex;
}


namespace tna
{

struct mutex_t
{
  std::mutex* p_mutex_impl;
};
  

void
mutex_init(mutex_t* mutex);

void
mutex_release(mutex_t* mutex);

void
mutex_lock(mutex_t*  mutex);

void
mutex_unlock(mutex_t* mutex);

} /* tna */ 


#endif /* ifndef _TNA_TASKING_MUTEX_H_H */
