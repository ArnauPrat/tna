
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

struct render_mesh_t 
{
  vector3_t colors[4] = {TNA_COLOR_RED, 
                         TNA_COLOR_BLUE, 
                         TNA_COLOR_GREEN, 
                         TNA_COLOR_BLACK};

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

    material_desc_t material;
    rendering_scene_get_material(p_rendering_scene,
                                 mesh->m_handler, 
                                 &material);

    material.m_color = colors[context->m_thread_id % 4];

    rendering_scene_set_material(p_rendering_scene,
                                 mesh->m_handler, 
                                 material);

  }
};

furious::match<render_mesh_data_t,global_transform_matrix_t,furious::Global<projview_matrix_t>>().foreach<render_mesh_t>();

END_FURIOUS_SCRIPT


