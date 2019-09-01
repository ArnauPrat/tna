
#include "../engine.h"
#include "../math/matrix.h"
#include "../math/math_tools.h"
#include "../components/render_mesh_data.h"
#include "../rendering/mesh_data.h"
#include "../shapes/aabb.h"
#include "../components/transform_matrix.h"
#include "../components/proj_view_matrix.h"
#include "../components/fps_camera.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct TnaRenderMesh 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const render_mesh_data_t* mesh, 
           const global_transform_matrix_t* transform_matrix,
           const projview_matrix_t* proj_matrix) 
  {
    rendering_scene_set_model_mat(p_rendering_scene, 
                                   mesh->m_handler, 
                                   transform_matrix->m_matrix);

    rendering_scene_set_frustrum_culling(p_rendering_scene, 
                                         mesh->m_handler, 
                                         proj_matrix->m_matrix);
  }
};

furious::match<render_mesh_data_t,global_transform_matrix_t,furious::Global<projview_matrix_t>>().foreach<TnaRenderMesh>();

END_FURIOUS_SCRIPT


