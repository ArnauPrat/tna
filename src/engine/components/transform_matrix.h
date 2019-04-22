
#ifndef _TNA_ENGINE_MODEL_MATRIX_H_
#define _TNA_ENGINE_MODEL_MATRIX_H_

#include "../math/matrix.h"
#include <furious/components.h>

namespace tna 
{

struct TnaTransformMatrix 
{
  FURIOUS_COMPONENT(TnaTransformMatrix);

  TnaTransformMatrix() : 
  m_matrix(1.0f),
  m_dirty(true)
  {
  }

  TnaMatrix4  m_matrix;
  bool        m_dirty;
};
  
} /* tna */ 
#endif /* ifndef _TNA_TRANSFORM_H_ */
