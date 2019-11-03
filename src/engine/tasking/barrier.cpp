

#include "barrier.h"
#include "atomic_counter.h"
#include "tasking.h"

namespace tna 
{

struct barrier_yield_data_t
{
  barrier_t*  p_barrier;
  int32_t     m_num_processes;
};

static bool
do_resume(void* data)
{
  barrier_yield_data_t* yield_data = (barrier_yield_data_t*)data;
  int32_t res = (atomic_counter_get(&yield_data->p_barrier->m_counter) - yield_data->m_num_processes);
  return res >= 0;
}

void
barrier_t::wait(int32_t num_processes)
{
  atomic_counter_fetch_increment(&m_counter);
  uint32_t i = 0; 
  while(atomic_counter_get(&m_counter) - num_processes < 0)
  {
    if( i > 1000)
    {
      barrier_yield_data_t yield_data;
      yield_data.p_barrier = this;
      yield_data.m_num_processes = num_processes;
      tasking_yield(do_resume, &yield_data);
      i = 0;
    }
    ++i;
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

