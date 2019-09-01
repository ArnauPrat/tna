
#include "priorities.h"
#include "../game_references.h"
#include "../engine/engine.h"
#include "../engine/tools/references.h"
#include "../engine/math/matrix.h"
#include "../engine/math/math_tools.h"
#include "../engine/components/transform.h"
#include "../engine/components/transform_matrix.h"
#include "../components/unit_member.h"

#include <furious/lang/lang.h>
#include "stdio.h"

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct UpdateUnitMember 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TnaTransform* transform,
           UnitMember*   unit_member) 
  {

    TnaVector3 direction = {unit_member->m_target.x - transform->m_position.x,
                            unit_member->m_target.y - transform->m_position.y,
                            unit_member->m_target.z - transform->m_position.z};
    if(direction.x != 0 ||
       direction.y != 0 ||
       direction.z != 0)
    {
      float distance = length(direction);
      if(distance > 0.01 )
      {

        float speed_factor = 1.0; 
        if(unit_member->m_time_to_max_speed > 0.0)
        {
          speed_factor = 0.25;
          unit_member->m_time_to_max_speed -= context->m_dt;
        }

        direction = normalize(direction);
        float speed = unit_member->m_speed*speed_factor;
        transform->m_position.x += context->m_dt*direction.x*speed;
        transform->m_position.y += context->m_dt*direction.y*speed;
        transform->m_position.z += context->m_dt*direction.z*speed;
      }
      else 
      {
        transform->m_position.x = unit_member->m_target.x;
        transform->m_position.y = unit_member->m_target.y;
        transform->m_position.z = unit_member->m_target.z;
      }
      transform->m_dirty = true;
    }
    else
    {
      float delay =  (0.5*(((uint32_t)rand()) / (double) UINT_MAX))+((unit_member->m_row) / 2.0f);
      unit_member->m_time_to_max_speed = delay;
    }
  }
};


furious::match<TnaTransform, UnitMember>().foreach<UpdateUnitMember>()
                                          .set_priority(PRIORITY_UPDATE_UNIT_MEMBER);

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

struct PrepareUnitMember 
{
  void run(furious::Context* context, 
           uint32_t id, 
           UnitMember*  unit_member,
           const TnaGlobalTransformMatrix* global_unit_matrix) 
  {
    TnaVector4 target_position = global_unit_matrix->m_matrix*TnaVector4{unit_member->m_unit_position, 1.0};
    unit_member->m_target = {target_position.x, target_position.y, target_position.z};
  }
};

furious::match<UnitMember>().expand<TnaGlobalTransformMatrix>(TNA_GAME_REF_BELONGS_TO_UNIT)
                                          .foreach<PrepareUnitMember>()
                                          .set_post_frame();

END_FURIOUS_SCRIPT
