
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
           const TnaRenderMeshData* mesh, 
           const TnaTransformMatrix* transform_matrix,
           const TnaProjViewMatrix* proj_matrix) 
  {
    p_rendering_scene->set_model_mat(mesh->m_handler, transform_matrix->m_matrix);
    p_rendering_scene->set_frustrum_culling(mesh->m_handler, proj_matrix->m_matrix);
  }
};

furious::match<TnaRenderMeshData,TnaTransformMatrix,furious::Global<TnaProjViewMatrix>>().foreach<TnaRenderMesh>();

END_FURIOUS_SCRIPT


