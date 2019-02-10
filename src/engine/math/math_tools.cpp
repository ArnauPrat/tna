

#include "math_tools.h"
#include <glm/gtc/matrix_transform.hpp>


namespace tna
{
Matrix4
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

Matrix4 translate(const Matrix4& matrix,
                  const Vector3& translation)
{
  return glm::translate(matrix, translation);
}

Matrix4 rotate(const Matrix4& matrix,
               float angle,
               const Vector3& axis)
{
  return glm::rotate(matrix, angle, axis);
}

Matrix4
scale(const Matrix4& matrix, 
      const Vector3& scale)
{
  return glm::scale(matrix, scale);
}

Matrix4
look_at(const Vector3& eye,
        const Vector3& center,
        const Vector3& up)
{
  return glm::lookAt(eye, center, up);
}

}
