
#include "../engine.h"
#include "../tools/references.h"
#include "../math/matrix.h"
#include "../math/math_tools.h"
#include "../components/transform.h"
#include "../components/transform_matrix.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct TnaInitTransformMatrix 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TnaLocalTransformMatrix* local_matrix,
           TnaGlobalTransformMatrix* global_matrix,
           TnaTransform* transform) 
  {
    local_matrix->m_matrix = transform->to_matrix();
    local_matrix->m_dirty = true;
    global_matrix->m_matrix = local_matrix->m_matrix;
    transform->m_dirty = false;
  }
};

furious::match<TnaLocalTransformMatrix, TnaGlobalTransformMatrix, TnaTransform>().filter([](const TnaLocalTransformMatrix* local_matrix,
                                                                                            const TnaGlobalTransformMatrix* global_matrix,
                                                                                            const TnaTransform* transform)
                                                    {
                                                      return transform->m_dirty;
                                                    }
                                                    )
                                            .foreach<TnaInitTransformMatrix>()
                                            .set_priority(1000);

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

struct TnaTransformMatrixHierarchy 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TnaGlobalTransformMatrix* global_matrix,
           const TnaLocalTransformMatrix* local_matrix,
           const TnaLocalTransformMatrix* parent_matrix) 
  {
    global_matrix->m_matrix = parent_matrix->m_matrix * local_matrix->m_matrix;
  }
};


furious::match<TnaGlobalTransformMatrix, TnaLocalTransformMatrix>().expand<TnaLocalTransformMatrix>(TNA_REF_PARENT)
                                    .filter([] (const TnaGlobalTransformMatrix* global_matrix,
                                                const TnaLocalTransformMatrix* local_matrix,
                                                const TnaLocalTransformMatrix* parent_matrix)
                                         {
                                          return local_matrix->m_dirty || parent_matrix->m_dirty;
                                         }
                                         )
                                    .foreach<TnaTransformMatrixHierarchy>()
                                    .set_priority(1001);


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////


struct TnaResetLocalTransformMatrix
{
  void run(furious::Context* context, 
           uint32_t id, 
           TnaLocalTransformMatrix*  matrix) 
  {
    matrix->m_dirty = false;
  }
};

furious::match<TnaLocalTransformMatrix>().foreach<TnaResetLocalTransformMatrix>()
                                         .set_post_frame();

END_FURIOUS_SCRIPT


