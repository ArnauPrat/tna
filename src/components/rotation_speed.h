



#ifndef _TNA_ROTATION_SPEED_H
#define _TNA_ROTATION_SPEED_H value

#include <furious/components.h>

namespace tna
{

struct RotationSpeed 
{
  FURIOUS_COMPONENT(RotationSpeed);

  RotationSpeed(float speed) : m_speed{speed} {}

  float m_speed;
}; /* tna */ 

}



#endif /* ifndef _TNA_ROTATION_SPEED_H */
