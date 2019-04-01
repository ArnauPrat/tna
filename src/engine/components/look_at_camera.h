

#ifndef _TNA_ENGINE_LOOK_AT_CAMERA_H_
#define _TNA_ENGINE_LOOK_AT_CAMERA_H_ value

#include "../math/matrix.h"
#include "../math/vector.h"
#include "../resources/resources.h"
#include <furious/components.h>

namespace tna 
{

struct LookAtCamera 
{
  FURIOUS_COMPONENT(LookAtCamera);

  LookAtCamera(const Vector3& eye,
               const Vector3& target,
               const Vector3& up) :
  m_eye(eye),
  m_target(target),
  m_up(up)
  {
  }


  Vector3 m_eye;
  Vector3 m_target;
  Vector3 m_up;

};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
