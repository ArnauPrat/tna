

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

struct fps_camera_t 
{
  FURIOUS_COMPONENT(fps_camera_t);

  vector3_t m_eye  = vector3_t(0.0f,0.0f,0.0f);
  float m_pitch     = 0.0f;
  float m_yaw       = 0.0f;
  float m_speed     = 0.0f;
  float m_pitch_speed = radians(5.0);
  float m_yaw_speed   = radians(5.0);

};

void
fps_camera_init(fps_camera_t* camera, 
            const vector3_t& eye,
            float pitch,
            float yaw);

void 
fps_camera_to_view_matrix(const fps_camera_t* camera, 
               matrix4_t* matrix);

void
fps_camera_forward(fps_camera_t* camera, 
        float amount);

void
fps_camera_strafe(fps_camera_t* camera, float amount);

void
fps_camera_yaw(fps_camera_t* camera, float amount);

void
fps_camera_pitch(fps_camera_t* camera, float amount);
  
} /* tna */ 
#endif

