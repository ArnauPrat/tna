
#ifndef _TNA_MATH_TOOLS_H_
#define _TNA_MATH_TOOLS_H_

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "matrix.h"
#include "vector.h"

namespace tna
{

constexpr double PI=3.1415926535897932384626433;

TnaMatrix4
create_projection_matrix(float field_of_view,
                         float ratio, 
                         float near_plan, 
                         float far_plane);

TnaMatrix4 
translate(const TnaMatrix4& matrix,
          const TnaVector3& translation);

TnaMatrix4 
rotate(const TnaMatrix4& matrix,
       float angle,
       const TnaVector3& axis);

TnaMatrix4
scale(const TnaMatrix4& matrix, 
      const TnaVector3& scale);

TnaMatrix4
look_at(const TnaVector3& eye,
        const TnaVector3& center,
        const TnaVector3& up);

float 
radians(float angle);


TnaMatrix4
inverse(const TnaMatrix4 matrix);

float 
dot(const TnaVector3& a, const TnaVector3 b);

}


#endif
