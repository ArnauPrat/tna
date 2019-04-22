
#include "../engine.h"
#include "../components/look_at_camera.h"
#include "../components/fps_camera.h"
#include "../components/viewport.h"
#include "../components/proj_view_matrix.h"
#include "../math/math_tools.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct TnaSetProjView 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TnaProjViewMatrix* proj_view,
           const TnaFPSCamera* fps_camera,
           const TnaViewport* viewport) 
  {
    TnaMatrix4 view_mat;
    fps_camera->to_view_matrix(&view_mat);
    p_rendering_scene->set_view_matrix(view_mat);

    TnaMatrix4 proj_mat = create_projection_matrix(viewport->m_field,
                                                   viewport->m_width/(float)viewport->m_height, 
                                                   viewport->m_near, 
                                                   viewport->m_far);

    proj_mat[1][1] *= -1;
    p_rendering_scene->set_proj_matrix(proj_mat);

    proj_view->m_matrix = proj_mat*view_mat;
  }
};

furious::match<furious::Global<TnaProjViewMatrix>, furious::Global<TnaFPSCamera>, furious::Global<TnaViewport>>().foreach<TnaSetProjView>();

END_FURIOUS_SCRIPT
