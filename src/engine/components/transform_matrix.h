
#ifndef _TNA_ENGINE_MODEL_MATRIX_H_
#define _TNA_ENGINE_MODEL_MATRIX_H_

#include "../math/matrix.h"
#include <furious/components.h>

namespace tna 
{

struct local_transform_matrix_t 
{
  FURIOUS_COMPONENT(local_transform_matrix_t);

  matrix4_t  m_matrix = matrix4_t(1.0f);
  bool        m_dirty  = true;
};

struct global_transform_matrix_t 
{
  FURIOUS_COMPONENT(global_transform_matrix_t);

  matrix4_t  m_matrix = matrix4_t(1.0f);
  bool        m_dirty  = true;
};
  
} /* tna */ 
#endif /* ifndef _TNA_TRANSFORM_H_ */
