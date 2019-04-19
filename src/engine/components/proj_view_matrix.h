

#ifndef _TNA_ENGINE_PROJ_VIEW_MATRIX_H_
#define _TNA_ENGINE_PROJ_VIEW_MATRIX_H_

#include <furious/components.h>
#include "../math/matrix.h"

namespace tna 
{

struct ProjViewMatrix 
{
  FURIOUS_COMPONENT(ProjViewMatrix);
  
  Matrix4 m_matrix;
};
} /* tna */ 

#endif
