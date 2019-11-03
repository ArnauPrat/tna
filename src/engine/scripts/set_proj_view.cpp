
#include "../engine.h"
#include "../components/look_at_camera.h"
#include "../components/fps_camera.h"
#include "../components/viewport.h"
#include "../components/proj_view_matrix.h"
#include "../math/math_tools.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct set_proj_view_t 
{
  void run(furious::Context* context, 
           uint32_t id, 
           projview_matrix_t* proj_view,
           const fps_camera_t* fps_camera,
           const viewport_t* viewport) 
  {
    matrix4_t view_mat;
    fps_camera_to_view_matrix(fps_camera, &view_mat);
    rendering_scene_set_view_matrix(p_rendering_scene, view_mat);

    matrix4_t proj_mat = create_projection_matrix(viewport->m_field,
                                                   viewport->m_width/(float)viewport->m_height, 
                                                   viewport->m_near, 
                                                   viewport->m_far);

    proj_mat[1][1] *= -1;
    rendering_scene_set_proj_matrix(p_rendering_scene, proj_mat);

    proj_view->m_matrix = proj_mat*view_mat;
  }
};

furious::match<furious::Global<projview_matrix_t>, furious::Global<fps_camera_t>, furious::Global<viewport_t>>().foreach<set_proj_view_t>();

END_FURIOUS_SCRIPT
