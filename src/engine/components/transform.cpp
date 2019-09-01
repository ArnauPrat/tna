

#include "transform.h"

namespace tna
{

void
transform_init(transform_t* transform, 
               const vector3_t& position) 
{
  transform->m_position = position;
  transform->m_local_rotation = vector3_t(0.0f, 0.0f, 0.0f);
  transform->m_global_rotation = vector3_t(0.0f, 0.0f, 0.0f);
  transform->m_scale = vector3_t(1.0f, 1.0f, 1.0f);
  transform->m_dirty = true;
}

void
transform_init(transform_t* transform, 
               const vector3_t& position,
               const vector3_t& rotation) 
{
  transform->m_position = position;
  transform->m_local_rotation = rotation;
  transform->m_global_rotation = vector3_t(0.0f, 0.0f, 0.0f);
  transform->m_scale = vector3_t(1.0f, 1.0f, 1.0f);
  transform->m_dirty = true;
}

void
transform_init(transform_t* transform, 
               const vector3_t& position, 
               const vector3_t& rotation, 
               const vector3_t& scale) 
{
  transform->m_position = position;
  transform->m_local_rotation = rotation;
  transform->m_global_rotation = vector3_t(0.0f, 0.0f, 0.0f);
  transform->m_scale = scale;
  transform->m_dirty = true;
}



matrix4_t
transform_to_matrix(transform_t* transform)
{
  matrix4_t mat(1.0f);
  mat = rotate(mat, transform->m_global_rotation.x, vector3_t(1.0, 0.0, 0.0));
  mat = rotate(mat, transform->m_global_rotation.y, vector3_t(0.0, 1.0, 0.0));
  mat = rotate(mat, transform->m_global_rotation.z, vector3_t(0.0, 0.0, 1.0));
  mat = translate(mat, transform->m_position);
  mat = rotate(mat, transform->m_local_rotation.x, vector3_t(1.0, 0.0, 0.0));
  mat = rotate(mat, transform->m_local_rotation.y, vector3_t(0.0, 1.0, 0.0));
  mat = rotate(mat, transform->m_local_rotation.z, vector3_t(0.0, 0.0, 1.0));
  mat = scale(mat, transform->m_scale);
  return mat;
}

} /* tna */ 
