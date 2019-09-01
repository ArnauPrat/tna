

#ifndef _TNA_ENGINE_LOOK_AT_CAMERA_H_
#define _TNA_ENGINE_LOOK_AT_CAMERA_H_ value

#include "../math/matrix.h"
#include "../math/vector.h"
#include "../resources/resources.h"
#include <furious/components.h>

namespace tna 
{

struct look_at_camera_t 
{
  FURIOUS_COMPONENT(look_at_camera_t);

  vector3_t m_eye    = vector3_t(1.0f, 1.0f, 1.0f);
  vector3_t m_target = vector3_t(0.0f, 0.0f, 0.0f);
  vector3_t m_up     = vector3_t(0.0f, 1.0f, 0.0f);

};

void
look_at_camera_init(look_at_camera_t* camera, 
                    const vector3_t& eye,
                    const vector3_t& target,
                    const vector3_t& up);

  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
