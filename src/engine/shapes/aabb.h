


#ifndef _TNA_SHAPE_AABB_H_
#define _TNA_SHAPE_AABB_H_

#include "../math/vector.h"


namespace  tna
{

struct aabb_t 
{
  vector3_t m_min = vector3_t(0.0f, 0.0f, 0.0f);
  vector3_t m_max = vector3_t(1.0f, 1.0f, 1.0f);
};
  
} /*  tna */ 


#endif /* ifndef _TNA_SHAPE_AABB_H_ */
