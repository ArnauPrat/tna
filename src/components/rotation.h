
#ifndef _TNA_ROTATION_H_
#define _TNA_ROTATION_H_ value
#include "engine/engine.h"

namespace furious
{
class Context;
}


namespace tna 
{

struct Rotation 
{
  void 
  run(furious::Context* context, 
           int32_t id, Transform* transform) 
  {
    //transform->m_rotation.y = context->get_dt()*glm::radians(90.0f);
  }
};
  
} /* tna */ 

#endif /* ifndef _TNA_ROTATION_H_ */
