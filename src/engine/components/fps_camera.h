

#ifndef _TNA_ENGINE_FPSCAMERA_H_
#define _TNA_ENGINE_FPSCAMERA_H_

#include "../math/matrix.h"
#include "../math/vector.h"
#include "../math/math_tools.h"
#include "../resources/resources.h"
#include <furious/components.h>

#include <math.h>

namespace tna 
{

struct TnaFPSCamera 
{
  FURIOUS_COMPONENT(TnaFPSCamera);

  TnaFPSCamera(const TnaVector3& eye,
               float pitch,
               float yaw) :
  m_eye(eye),
  m_pitch(pitch),
  m_yaw(yaw),
  m_speed(1.0),
  m_pitch_speed(radians(5.0)),
  m_yaw_speed(radians(5.0))
  {
  }

  void 
  to_view_matrix(TnaMatrix4* matrix) const
  {
    float sinPitch = sin(m_pitch);
    float cosPitch = cos(m_pitch);
    float sinYaw = sin(m_yaw);
    float cosYaw = cos(m_yaw);
    TnaVector3 xaxis = { cosYaw, 0, -sinYaw };
    TnaVector3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
    TnaVector3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
    (*matrix)[0] = {xaxis.x, yaxis.x, zaxis.x, 0};
    (*matrix)[1] = {xaxis.y, yaxis.y, zaxis.y, 0};
    (*matrix)[2] = {xaxis.z, yaxis.z, zaxis.z, 0};
    (*matrix)[3] = {-dot(xaxis, m_eye), -dot(yaxis, m_eye), -dot(zaxis, m_eye), 1};
  }

  void
  forward(float amount)
  {
    float sinPitch = sin(m_pitch);
    float cosPitch = cos(m_pitch);
    float sinYaw = sin(m_yaw);
    float cosYaw = cos(m_yaw);
    TnaVector3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
    m_eye += -m_speed*amount*zaxis;
  }

  void
  strafe(float amount)
  {
    float sinYaw = sin(m_yaw);
    float cosYaw = cos(m_yaw);
    TnaVector3 xaxis = { cosYaw, 0, -sinYaw };
    m_eye += m_speed*amount*xaxis;
  }

  void
  yaw(float amount)
  {
    m_yaw += -m_yaw_speed*radians(amount);
  }

  void
  pitch(float amount)
  {
    m_pitch += -m_pitch_speed*radians(amount);
  }

  TnaVector3 m_eye;
  float m_pitch;
  float m_yaw;
  float m_speed;
  float m_pitch_speed;
  float m_yaw_speed;

};
  
} /* tna */ 
#endif

