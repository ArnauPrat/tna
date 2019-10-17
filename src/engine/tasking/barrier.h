
#ifndef _TNA_TASKING_BARRIER_H
#define _TNA_TASKING_BARRIER_H value

#include "atomic_counter.h"
#include <furious/furious.h>

namespace tna
{

struct barrier_t : public furious::barrier_t
{
  void
  wait(int32_t value);

  void
  reset();

  atomic_counter_t  m_counter;
};

void
barrier_init(barrier_t* barrier);

void
barrier_release(barrier_t* barrier);

void
barrier_reset(barrier_t* barrier);
  
} /* tna */ 

#endif /* ifndef _TNA_TASKING_BARRIER_H */
