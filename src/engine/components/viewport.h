

#ifndef _TNA_ENGINE_VIEWPORT_H_
#define _TNA_ENGINE_VIEWPORT_H_

#include <furious/components.h>
#include "../math/vector.h"

namespace tna 
{

struct viewport_t 
{
  FURIOUS_COMPONENT(viewport_t);

  uint32_t  m_width     = 900;
  uint32_t  m_height    = 1440;
  float     m_near      = 0.1f;
  float     m_far       = 1000.0f;
  float     m_field     = 60.0f;

};
  
} /* tna */ 

#endif /* ifndef _TNA_FRUSTRUM_H_ */
