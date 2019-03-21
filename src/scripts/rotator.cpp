
#include "../engine/engine.h"
#include "../engine/math/matrix.h"
#include "../engine/math/math_tools.h"
#include "../engine/components/transform.h"

#include <furious/lang/lang.h>
#include "stdio.h"

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct Rotator 
{
  Rotator(float speed) : 
  m_speed(speed)
  {
  }
  
  void run(furious::Context* context, 
           uint32_t id, 
           Transform* transform) 
  {
    float angle = context->m_dt*m_speed*360.0;
    transform->m_rotation.y += angle;
  }
  float m_speed;
};

furious::match<Transform>().foreach<Rotator>(1.0);

END_FURIOUS_SCRIPT
