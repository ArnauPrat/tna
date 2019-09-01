

#ifndef _TNA_ENGINE_PROJ_VIEW_MATRIX_H_
#define _TNA_ENGINE_PROJ_VIEW_MATRIX_H_

#include <furious/components.h>
#include "../math/matrix.h"

namespace tna 
{

struct projview_matrix_t 
{
  FURIOUS_COMPONENT(projview_matrix_t);
  
  matrix4_t m_matrix = matrix4_t(1.0f);
};
} /* tna */ 

#endif
