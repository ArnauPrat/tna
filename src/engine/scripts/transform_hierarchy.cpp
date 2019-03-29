
#include "../engine.h"
#include "../math/matrix.h"
#include "../math/math_tools.h"
#include "../components/transform.h"
#include "../components/transform_matrix.h"
#include "../components/mesh.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct InitTransformMatrix 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TransformMatrix* model_matrix,
           const Transform* transform) 
  {
    model_matrix->m_matrix = Matrix4(1.0f);
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_global_rotation.x, Vector3(1.0, 0.0, 0.0));
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_global_rotation.y, Vector3(0.0, 1.0, 0.0));
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_global_rotation.z, Vector3(0.0, 0.0, 1.0));
    model_matrix->m_matrix = translate(model_matrix->m_matrix, transform->m_position);
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_local_rotation.x, Vector3(1.0, 0.0, 0.0));
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_local_rotation.y, Vector3(0.0, 1.0, 0.0));
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_local_rotation.z, Vector3(0.0, 0.0, 1.0));
    model_matrix->m_matrix = scale(model_matrix->m_matrix, transform->m_scale);
  }
};

struct TransformMatrixHierarchy 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TransformMatrix* model_matrix,
           const TransformMatrix* parent_matrix) 
  {
    model_matrix->m_matrix = parent_matrix->m_matrix * model_matrix->m_matrix;
  }
};

furious::match<TransformMatrix, Transform>().foreach<InitTransformMatrix>()
                                            .set_priority(2);

furious::match<TransformMatrix>().expand<TransformMatrix>("parent")
                                 .foreach<TransformMatrixHierarchy>()
                                 .set_priority(0);

END_FURIOUS_SCRIPT


