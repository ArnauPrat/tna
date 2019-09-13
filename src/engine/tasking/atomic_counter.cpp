

#include "atomic_counter.h"
#include "tasking.h"
#include <atomic>
#include <thread>


namespace tna
{
  

void
atomic_counter_init(atomic_counter_t* counter) 
{
  counter->p_impl = new std::atomic<int32_t>();
  counter->p_impl->store(0);
}

void
atomic_counter_release(atomic_counter_t* counter)
{
  if(counter->p_impl)
  {
    delete counter->p_impl;
    counter->p_impl = nullptr;
  }
}

void 
atomic_counter_set(atomic_counter_t* counter,
                        int32_t value) 
{
  counter->p_impl->store(value);
}

int32_t 
atomic_counter_fetch_increment(atomic_counter_t* counter) 
{
  return counter->p_impl->fetch_add(1);
}

int32_t atomic_counter_fetch_decrement(atomic_counter_t* counter)
{
  return counter->p_impl->fetch_add(-1);
}

void atomic_counter_join(atomic_counter_t* counter) 
{
  while(counter->p_impl->load() != 0) 
  {
    if(get_current_thread_id() == INVALID_THREAD_ID) 
    { // if this is a non-pool thread
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } 
    else 
    { // if this is a lightweight thread
      yield();
    }
  }
}


} /* tna */ 
