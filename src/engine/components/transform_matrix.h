
#ifndef _TNA_ENGINE_MODEL_MATRIX_H_
#define _TNA_ENGINE_MODEL_MATRIX_H_

#include "../math/matrix.h"
#include <furious/components.h>

namespace tna 
{

struct TransformMatrix 
{
  FURIOUS_COMPONENT(TransformMatrix);

  TransformMatrix() : 
  m_matrix(1.0f)
  {
  }

  Matrix4 m_matrix;
};
  
} /* tna */ 
#endif /* ifndef _TNA_TRANSFORM_H_ */
