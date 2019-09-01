
#include "look_at_camera.h"

namespace tna
{

void
look_at_camera_init(look_at_camera_t* camera, 
                    const TnaVector3& eye,
                    const TnaVector3& target,
                    const TnaVector3& up)
{
  camera->m_eye= eye;
  camera->m_target= target;
  camera->m_up= up;
}
  
} /* tna */ 
