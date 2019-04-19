
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

Matrix4
create_projection_matrix(float field_of_view,
                         float ratio, 
                         float near_plan, 
                         float far_plane);

Matrix4 
translate(const Matrix4& matrix,
          const Vector3& translation);

Matrix4 
rotate(const Matrix4& matrix,
       float angle,
       const Vector3& axis);

Matrix4
scale(const Matrix4& matrix, 
      const Vector3& scale);

Matrix4
look_at(const Vector3& eye,
        const Vector3& center,
        const Vector3& up);

float 
radians(float angle);


Matrix4
inverse(const Matrix4 matrix);

float 
dot(const Vector3& a, const Vector3 b);

}


#endif
