

#ifndef _TNA_ENGINE_TRANSFORM_H_
#define _TNA_ENGINE_TRANSFORM_H_

#include "../math/vector.h"
#include "../math/math_tools.h"
#include <furious/components.h>

namespace tna 
{

struct TnaTransform 
{
  FURIOUS_COMPONENT(TnaTransform);

  TnaVector3  m_position;
  TnaVector3  m_local_rotation;
  TnaVector3  m_global_rotation;
  TnaVector3  m_scale;
  bool        m_dirty;

  TnaTransform() : 
  m_position(0.0f, 0.0f, 0.0f),
  m_local_rotation(0.0f, 0.0f, 0.0f),
  m_global_rotation(0.0f, 0.0f, 0.0f),
  m_scale(1.0f, 1.0f, 1.0f),
  m_dirty(true)
  {

  }

  TnaTransform(const TnaVector3& position) :
  m_position(position),
  m_local_rotation(0.0f, 0.0f, 0.0f),
  m_global_rotation(0.0f, 0.0f, 0.0f),
  m_scale(1.0f, 1.0f, 1.0f),
  m_dirty(true)
  {
  }

  TnaTransform(const TnaVector3& position, const TnaVector3& rotation) : 
  m_position(position),
  m_local_rotation(rotation),
  m_global_rotation(0.0f, 0.0f, 0.0f),
  m_scale(0.0f, 0.0f, 0.0f),
  m_dirty(true)
  {
  }

  TnaTransform(const TnaVector3& position, const TnaVector3& rotation, const TnaVector3& scale) :
  m_position(position),
  m_local_rotation(rotation),
  m_global_rotation(rotation),
  m_scale(scale),
  m_dirty(true)
  {
  }

  TnaMatrix4
  to_matrix() const
  {
    TnaMatrix4 mat(1.0f);
    mat = rotate(mat, m_global_rotation.x, TnaVector3(1.0, 0.0, 0.0));
    mat = rotate(mat, m_global_rotation.y, TnaVector3(0.0, 1.0, 0.0));
    mat = rotate(mat, m_global_rotation.z, TnaVector3(0.0, 0.0, 1.0));
    mat = translate(mat, m_position);
    mat = rotate(mat, m_local_rotation.x, TnaVector3(1.0, 0.0, 0.0));
    mat = rotate(mat, m_local_rotation.y, TnaVector3(0.0, 1.0, 0.0));
    mat = rotate(mat, m_local_rotation.z, TnaVector3(0.0, 0.0, 1.0));
    mat = scale(mat, m_scale);
    return mat;
  }


};
  
} /* tna */ 
#endif /* ifndef _TNA_TRANSFORM_H_ */
