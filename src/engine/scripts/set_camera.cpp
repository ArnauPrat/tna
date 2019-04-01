
#include "../engine.h"
#include "../components/look_at_camera.h"
#include "../components/fps_camera.h"
#include "../math/math_tools.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct SetLookAtCamera 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const LookAtCamera* look_at_camera) 
  {
    Matrix4 mat = look_at(look_at_camera->m_eye,
                          look_at_camera->m_target,
                          look_at_camera->m_up);
    set_camera(&mat);
  }
};

struct SetFPSCamera 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const FPSCamera* fps_camera) 
  {
    Matrix4 mat;
    fps_camera->to_view_matrix(&mat);
    set_camera(&mat);
  }
};

//furious::match<LookAtCamera>().foreach<SetLookAtCamera>();
furious::match<FPSCamera>().foreach<SetFPSCamera>();

END_FURIOUS_SCRIPT
