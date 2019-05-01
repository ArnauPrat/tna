
#include "../engine/engine.h"
#include "../engine/tools/references.h"
#include "../engine/math/matrix.h"
#include "../engine/math/math_tools.h"
#include "../engine/components/transform.h"
#include "../components/laggy_follower.h"

#include <furious/lang/lang.h>
#include "stdio.h"

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct LaggyFollow
{
  void run(furious::Context* context, 
           uint32_t id, 
           LaggyFollower* follower,
           TnaTransform* transform,
           const TnaTransform* parent_transform) 
  {

    TnaVector3* position = &transform->m_position;
    const TnaVector3* parent_position = &parent_transform->m_position;
    TnaVector3* old_parent_position = &follower->m_parent_position;

    if(parent_position->x != old_parent_position->x || 
       parent_position->y != old_parent_position->y ||
       parent_position->z != old_parent_position->z )
    {
      if(follower->m_current_delay < follower->m_delay_to_full_speed)
      {
        TnaVector3 direction = {0.0,0.0,-1.0};
        position->x += context->m_dt*direction.x;
        position->y += context->m_dt*direction.y;
        position->z += context->m_dt*direction.z;

        follower->m_current_delay += context->m_dt;
      }
    }
    else
    {
      TnaVector3* rest_position = &follower->m_rest_position;
      TnaVector3 direction = {rest_position->x - position->x,
                              rest_position->y - position->y,
                              rest_position->z - position->z};

      if(direction.x != 0.0 ||
         direction.y != 0.0 ||
         direction.z != 0.0)
      {
        float distance = length(direction);
        if(distance >= 0.1)
        {
          direction = normalize(direction);
          position->x += context->m_dt*direction.x;
          position->y += context->m_dt*direction.y;
          position->z += context->m_dt*direction.z;
        }
        else
        {
          position->x = rest_position->x;
          position->y = rest_position->y;
          position->z = rest_position->z;

          follower->m_current_delay -= context->m_dt;
          if(follower->m_current_delay < 0.0)
          {
            follower->m_current_delay = 0.0;
          }
        }
      }
      else
      {
          position->x = rest_position->x;
          position->y = rest_position->y;
          position->z = rest_position->z;

          follower->m_current_delay -= context->m_dt;
          if(follower->m_current_delay < 0.0)
          {
            follower->m_current_delay = 0.0;
          }
      }
    }

    old_parent_position->x = parent_position->x;
    old_parent_position->y = parent_position->y;
    old_parent_position->z = parent_position->z;

    transform->m_dirty = true;
  }
};

furious::match<LaggyFollower, TnaTransform>().expand<TnaTransform>(TNA_REF_PARENT)
                               .foreach<LaggyFollow>();

END_FURIOUS_SCRIPT
