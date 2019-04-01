
#include "../engine.h"
#include "../math/matrix.h"
#include "../math/math_tools.h"
#include "../components/mesh.h"
#include "../components/transform_matrix.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct RenderMesh 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const Mesh* mesh, 
           const TransformMatrix* transform_matrix) 
  {
    render_mesh(mesh->m_mesh_data, 
                &transform_matrix->m_matrix,
                &mesh->m_color);
  }
};

furious::match<Mesh,TransformMatrix>().foreach<RenderMesh>();

END_FURIOUS_SCRIPT


