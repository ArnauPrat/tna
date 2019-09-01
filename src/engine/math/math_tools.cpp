

#include "math_tools.h"
#include <glm/gtc/matrix_transform.hpp>


namespace tna
{
matrix4_t
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

matrix4_t translate(const matrix4_t& matrix,
                  const vector3_t& translation)
{
  return glm::translate(matrix, translation);
}

matrix4_t rotate(const matrix4_t& matrix,
               float angle,
               const vector3_t& axis)
{
  return glm::rotate(matrix, angle, axis);
}

matrix4_t
scale(const matrix4_t& matrix, 
      const vector3_t& scale)
{
  return glm::scale(matrix, scale);
}

matrix4_t
look_at(const vector3_t& eye,
        const vector3_t& center,
        const vector3_t& up)
{
  return glm::lookAt(eye, center, up);
}

float 
radians(float angle)
{
  return glm::radians(angle);
}

matrix4_t
inverse(const matrix4_t matrix)
{
  return glm::inverse(matrix);
}

float 
dot(const vector3_t& a, const vector3_t b)
{
  return glm::dot(a,b);
}

float
length(const vector3_t& vect)
{
  return glm::length(vect);
}

vector3_t
normalize(const vector3_t& vect)
{
  return glm::normalize(vect);
}

}
