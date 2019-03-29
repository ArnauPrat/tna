

#ifndef _TNA_ENGINE_TRANSFORM_H_
#define _TNA_ENGINE_TRANSFORM_H_

#include "../math/vector.h"
#include <furious/components.h>

namespace tna 
{

struct Transform 
{
  FURIOUS_COMPONENT(Transform);

  Vector3 m_position;
  Vector3 m_local_rotation;
  Vector3 m_global_rotation;
  Vector3 m_scale;

  Transform() : 
  m_position(0.0f, 0.0f, 0.0f),
  m_local_rotation(0.0f, 0.0f, 0.0f),
  m_global_rotation(0.0f, 0.0f, 0.0f),
  m_scale(1.0f, 1.0f, 1.0f)
  {

  }

  Transform(const Vector3& position) :
  m_position(position),
  m_local_rotation(0.0f, 0.0f, 0.0f),
  m_global_rotation(0.0f, 0.0f, 0.0f),
  m_scale(1.0f, 1.0f, 1.0f)
  {
  }

  Transform(const Vector3& position, const Vector3& rotation) : 
  m_position(position),
  m_local_rotation(rotation),
  m_global_rotation(0.0f, 0.0f, 0.0f),
  m_scale(0.0f, 0.0f, 0.0f)
  {
  }

  Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale) :
  m_position(position),
  m_local_rotation(rotation),
  m_global_rotation(rotation),
  m_scale(scale)
  {
  }
};
  
} /* tna */ 
#endif /* ifndef _TNA_TRANSFORM_H_ */
