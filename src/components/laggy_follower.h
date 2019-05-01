
#ifndef _TNA_LAGGY_FOLLOWER_H_
#define _TNA_LAGGY_FOLLOWER_H_ value

#include <furious/components.h>

#include "../engine/math/vector.h"

namespace tna
{

struct LaggyFollower 
{
  FURIOUS_COMPONENT(LaggyFollower);

  LaggyFollower(const TnaVector3& parent_position,
                const TnaVector3& rest_position,
                float delay_to_full_speed) :
  m_parent_position(parent_position),
  m_rest_position(rest_position),
  m_delay_to_full_speed(delay_to_full_speed),
  m_current_delay(0.0)
  {

  }

  TnaVector3 m_parent_position;
  TnaVector3 m_rest_position;
  float      m_delay_to_full_speed;
  float      m_current_delay;
}; /* tna */ 

}


#endif /* ifndef _TNA_LAGGY_FOLLOWER_H_*/
