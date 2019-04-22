

#ifndef _TNA_ENGINE_VIEWPORT_H_
#define _TNA_ENGINE_VIEWPORT_H_

#include <furious/components.h>
#include "../math/vector.h"

namespace tna 
{

struct TnaViewport 
{
  FURIOUS_COMPONENT(TnaViewport);

  uint32_t  m_width;
  uint32_t  m_height;
  float     m_near;
  float     m_far;
  float     m_field;
};
  
} /* tna */ 

#endif /* ifndef _TNA_FRUSTRUM_H_ */
