

#ifndef _TNA_ENGINE_TRANSFORM_H_
#define _TNA_ENGINE_TRANSFORM_H_

#include "../math/vector.h"
#include "../math/math_tools.h"
#include <furious/components.h>

namespace tna 
{

struct transform_t 
{
  FURIOUS_COMPONENT(transform_t);

  vector3_t  m_position        = vector3_t(0.0f, 0.0f, 0.0f);
  vector3_t  m_local_rotation  = vector3_t(0.0f, 0.0f, 0.0f);
  vector3_t  m_global_rotation = vector3_t(0.0f, 0.0f, 0.0f);
  vector3_t  m_scale           = vector3_t(1.0f, 1.0f, 1.0f);
  bool        m_dirty           = true;
};

void
transform_init(transform_t* transform, 
               const vector3_t& position); 

void
transform_init(transform_t* transform, 
               const vector3_t& position,
               const vector3_t& rotation); 

void
transform_init(transform_t* transform, 
               const vector3_t& position, 
               const vector3_t& rotation, 
               const vector3_t& scale); 
void
transform_to_matrix(const transform_t* tranform, matrix4_t* output);

} /* tna */ 
#endif /* ifndef _TNA_TRANSFORM_H_ */
