
#ifndef _TNA_UNIT_MEMBER_H_
#define _TNA_UNIT_MEMBER_H_ value

#include <furious/components.h>

#include "../engine/math/vector.h"

namespace tna
{

struct unit_member_t
{
  FURIOUS_COMPONENT(unit_member_t);

  vector3_t   m_unit_position;
  vector3_t   m_target;
  float       m_speed;
  int32_t     m_row;
  int32_t     m_column;
  float       m_time_to_max_speed;
}; /* tna */ 

}


#endif /* ifndef _TNA_UNIT_MEMBER_H_*/
