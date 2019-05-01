

#include "math_tools.h"
#include <glm/gtc/matrix_transform.hpp>


namespace tna
{
TnaMatrix4
create_projection_matrix(float field_of_view,
                         float aspect_ratio, 
                         float near_plane, 
                         float far_plane)
{
  return glm::perspective(glm::radians(field_of_view), 
                          aspect_ratio, 
                          near_plane, 
                          far_plane);
}

TnaMatrix4 translate(const TnaMatrix4& matrix,
                  const TnaVector3& translation)
{
  return glm::translate(matrix, translation);
}

TnaMatrix4 rotate(const TnaMatrix4& matrix,
               float angle,
               const TnaVector3& axis)
{
  return glm::rotate(matrix, angle, axis);
}

TnaMatrix4
scale(const TnaMatrix4& matrix, 
      const TnaVector3& scale)
{
  return glm::scale(matrix, scale);
}

TnaMatrix4
look_at(const TnaVector3& eye,
        const TnaVector3& center,
        const TnaVector3& up)
{
  return glm::lookAt(eye, center, up);
}

float 
radians(float angle)
{
  return glm::radians(angle);
}

TnaMatrix4
inverse(const TnaMatrix4 matrix)
{
  return glm::inverse(matrix);
}

float 
dot(const TnaVector3& a, const TnaVector3 b)
{
  return glm::dot(a,b);
}

float
length(const TnaVector3& vect)
{
  return glm::length(vect);
}

TnaVector3
normalize(const TnaVector3& vect)
{
  return glm::normalize(vect);
}

}
