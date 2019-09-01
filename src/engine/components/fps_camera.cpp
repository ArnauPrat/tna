
#include "fps_camera.h"

namespace tna
{

void
fps_camera_init(fps_camera_t* camera, 
                const vector3_t& eye,
                float pitch,
                float yaw) 
{
  camera->m_eye = eye;
  camera->m_pitch = pitch;
  camera->m_yaw = yaw;
  camera->m_speed = 1.0;
  camera->m_pitch_speed = radians(5.0);
  camera->m_yaw_speed = radians(5.0);
}

void 
fps_camera_to_view_matrix(const fps_camera_t* camera, 
                          matrix4_t* matrix)
{
  float sinPitch = sin(camera->m_pitch);
  float cosPitch = cos(camera->m_pitch);
  float sinYaw = sin(camera->m_yaw);
  float cosYaw = cos(camera->m_yaw);
  vector3_t xaxis = { cosYaw, 0, -sinYaw };
  vector3_t yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
  vector3_t zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
  (*matrix)[0] = {xaxis.x, yaxis.x, zaxis.x, 0};
  (*matrix)[1] = {xaxis.y, yaxis.y, zaxis.y, 0};
  (*matrix)[2] = {xaxis.z, yaxis.z, zaxis.z, 0};
  (*matrix)[3] = {-dot(xaxis, camera->m_eye), -dot(yaxis, camera->m_eye), -dot(zaxis, camera->m_eye), 1};
}

void
fps_camera_forward(fps_camera_t* camera, 
                   float amount)
{
  float sinPitch = sin(camera->m_pitch);
  float cosPitch = cos(camera->m_pitch);
  float sinYaw = sin(camera->m_yaw);
  float cosYaw = cos(camera->m_yaw);
  vector3_t zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
  camera->m_eye += -camera->m_speed*amount*zaxis;
}

void
fps_camera_strafe(fps_camera_t* camera, float amount)
{
  float sinYaw = sin(camera->m_yaw);
  float cosYaw = cos(camera->m_yaw);
  vector3_t xaxis = { cosYaw, 0, -sinYaw };
  camera->m_eye += camera->m_speed*amount*xaxis;
}

void
fps_camera_yaw(fps_camera_t* camera, float amount)
{
  camera->m_yaw += -camera->m_yaw_speed*radians(amount);
}

void
fps_camera_pitch(fps_camera_t* camera, float amount)
{
  camera->m_pitch += -camera->m_pitch_speed*radians(amount);
}
}
