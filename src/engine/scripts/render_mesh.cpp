
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

struct RenderMesh 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const RenderMeshData* mesh, 
           const TransformMatrix* transform_matrix,
           const ProjViewMatrix* proj_matrix) 
  {
    const AABB* aabb = &mesh->p_mesh_data->m_aabb;
    Vector4 min = transform_matrix->m_matrix*Vector4(aabb->m_min,1.0f);
    Vector4 max = transform_matrix->m_matrix*Vector4(aabb->m_max,1.0f);
    Vector4 points[8] = {
      {min.x, min.y, min.z, 1.0f},
      {min.x, min.y, max.z, 1.0f},
      {min.x, max.y, min.z, 1.0f},
      {min.x, max.y, max.z, 1.0f},
      {max.x, min.y, min.z, 1.0f},
      {max.x, min.y, max.z, 1.0f},
      {max.x, max.y, min.z, 1.0f},
      {max.x, max.y, max.z, 1.0f}
    };
    bool visible = false;
    for(uint32_t i = 0; i < 8; ++i)
    {
      Vector4 proj_point = proj_matrix->m_matrix*points[i];
      if((-proj_point.w <= proj_point.x) && (proj_point.x <= proj_point.w) &&
         (-proj_point.w <= proj_point.y) && (proj_point.y <= proj_point.w) &&
         (0 <= proj_point.z) && (proj_point.z <= proj_point.w))
      {
        visible = true;
        break;
      }
    }
    
    RenderMeshDescriptor desc;
    desc.p_mesh_data = mesh->p_mesh_data;
    desc.m_placement.m_model_mat = transform_matrix->m_matrix;
    desc.m_material = mesh->m_material;
    //desc.m_placement.m_frustrum_visible = true;
    desc.m_placement.m_frustrum_visible = visible;
    render_mesh(desc);
  }
};

furious::match<RenderMeshData,TransformMatrix,furious::Global<ProjViewMatrix>>().foreach<RenderMesh>();

END_FURIOUS_SCRIPT


