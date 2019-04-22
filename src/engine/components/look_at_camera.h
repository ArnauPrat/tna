

#ifndef _TNA_ENGINE_LOOK_AT_CAMERA_H_
#define _TNA_ENGINE_LOOK_AT_CAMERA_H_ value

#include "../math/matrix.h"
#include "../math/vector.h"
#include "../resources/resources.h"
#include <furious/components.h>

namespace tna 
{

struct TnaLookAtCamera 
{
  FURIOUS_COMPONENT(TnaLookAtCamera);

  TnaLookAtCamera(const TnaVector3& eye,
                  const TnaVector3& target,
                  const TnaVector3& up) :
  m_eye(eye),
  m_target(target),
  m_up(up)
  {
  }

  TnaVector3 m_eye;
  TnaVector3 m_target;
  TnaVector3 m_up;

};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
