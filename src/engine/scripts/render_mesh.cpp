
#include "../engine.h"
#include "../math/matrix.h"
#include "../math/math_tools.h"
#include "../components/transform.h"
#include "../components/mesh.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct RenderMesh 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const Mesh* mesh, 
           const Transform* transform) {

    Matrix4 identity(1.0f);
    Matrix4 trans = translate(identity, transform->m_position);
    Matrix4 rotateX = rotate(trans, transform->m_rotation.x, Vector3(1.0, 0.0, 0.0));
    Matrix4 rotateY = rotate(rotateX, transform->m_rotation.y, Vector3(0.0, 1.0, 0.0));
    Matrix4 rotateZ = rotate(rotateY, transform->m_rotation.z, Vector3(0.0, 0.0, 1.0));
    Matrix4 model = scale(rotateZ, transform->m_scale);
    render_mesh(mesh->m_mesh_data, &model);
  }
};

furious::select<Mesh,Transform>().foreach<RenderMesh>();

END_FURIOUS_SCRIPT


