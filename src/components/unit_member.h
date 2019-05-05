
#ifndef _TNA_UNIT_MEMBER_H_
#define _TNA_UNIT_MEMBER_H_ value

#include <furious/components.h>

#include "../engine/math/vector.h"

namespace tna
{

struct UnitMember
{
  FURIOUS_COMPONENT(UnitMember);

  UnitMember(const TnaVector3& unit_position,
             float speed,
             int row,
             int column) :
  m_unit_position(unit_position),
  m_speed(speed),
  m_row(row),
  m_column(column),
  m_time_to_max_speed(-1.0)
  {

  }

  TnaVector3  m_unit_position;
  float       m_speed;
  float       m_row;
  float       m_column;
  float       m_time_to_max_speed;
}; /* tna */ 

}


#endif /* ifndef _TNA_UNIT_MEMBER_H_*/
