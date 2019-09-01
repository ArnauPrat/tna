
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

matrix4_t
create_projection_matrix(float field_of_view,
                         float ratio, 
                         float near_plan, 
                         float far_plane);

matrix4_t 
translate(const matrix4_t& matrix,
          const vector3_t& translation);

matrix4_t 
rotate(const matrix4_t& matrix,
       float angle,
       const vector3_t& axis);

matrix4_t
scale(const matrix4_t& matrix, 
      const vector3_t& scale);

matrix4_t
look_at(const vector3_t& eye,
        const vector3_t& center,
        const vector3_t& up);

float 
radians(float angle);


matrix4_t
inverse(const matrix4_t matrix);

float 
dot(const vector3_t& a, const vector3_t b);

float
length(const vector3_t& vect);

vector3_t
normalize(const vector3_t& vect);

}


#endif
