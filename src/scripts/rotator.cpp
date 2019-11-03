
#include "priorities.h"
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

struct rotator_around_parent_t
{
  void run(furious::Context* context, 
           uint32_t id, 
           transform_t* transform,
           const rotation_speed_t* rspeed) 
  {
    float angle = context->m_dt*rspeed->m_speed;
    transform->m_global_rotation.y += angle;
    transform->m_dirty = true;
  }
  float m_speed;
};

furious::match<transform_t, rotation_speed_t>().expand<>(TNA_REF_PARENT)
                                             .foreach<rotator_around_parent_t>()
                                             .set_priority(PRIORITY_ROTATOR);

END_FURIOUS_SCRIPT
