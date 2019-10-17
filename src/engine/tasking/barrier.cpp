

#include "barrier.h"
#include "atomic_counter.h"

namespace tna 
{

void
barrier_t::wait(int32_t num_processes)
{
  atomic_counter_fetch_increment(&m_counter);
  while(atomic_counter_get(&m_counter) - num_processes < 0)
  {
  }
}

void
barrier_t::reset()
{
  barrier_reset(this);
}

void
barrier_init(barrier_t* barrier)
{
  atomic_counter_init(&barrier->m_counter);
  barrier_reset(barrier);
}

void
barrier_release(barrier_t* barrier)
{
  atomic_counter_release(&barrier->m_counter);
}

void
barrier_reset(barrier_t* barrier)
{
  atomic_counter_set(&barrier->m_counter, 0);
}
  
} /* barrier */ 

