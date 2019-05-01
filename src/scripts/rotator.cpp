
#include "../engine/engine.h"
#include "../engine/tools/references.h"
#include "../engine/math/matrix.h"
#include "../engine/math/math_tools.h"
#include "../engine/components/transform.h"
#include "../components/rotation_speed.h"

#include <furious/lang/lang.h>
#include "stdio.h"

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct RotatorAroundParent
{
  RotatorAroundParent()  
  {
  }
  
  void run(furious::Context* context, 
           uint32_t id, 
           TnaTransform* transform,
           const RotationSpeed* rspeed) 
  {
    float angle = context->m_dt*rspeed->m_speed;
    transform->m_global_rotation.y += angle;
    transform->m_dirty = true;
  }
  float m_speed;
};

furious::match<TnaTransform, RotationSpeed>().expand<>(TNA_REF_PARENT)
                                             .foreach<RotatorAroundParent>()
                                             .set_priority(10);

END_FURIOUS_SCRIPT
