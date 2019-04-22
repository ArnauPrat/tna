
#include "../engine.h"
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
           TnaTransformMatrix* model_matrix,
           TnaTransform* transform) 
  {
    model_matrix->m_matrix = transform->to_matrix();
    model_matrix->m_dirty = true;
    transform->m_dirty = false;
  }
};

furious::match<TnaTransformMatrix, TnaTransform>().has_not_tag("__tna_static")
                                            .filter([](const TnaTransformMatrix* model_matrix,
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
           TnaTransformMatrix* model_matrix,
           const TnaTransformMatrix* parent_matrix) 
  {
    model_matrix->m_matrix = parent_matrix->m_matrix * model_matrix->m_matrix;
    model_matrix->m_dirty = true;
  }
};


furious::match<TnaTransformMatrix>().expand<TnaTransformMatrix>("parent")
                                 .filter([] (const TnaTransformMatrix* model_matrix,
                                             const TnaTransformMatrix* parent_matrix)
                                         {
                                          return model_matrix->m_dirty || parent_matrix->m_dirty;
                                         }
                                         )
                                 .foreach<TnaTransformMatrixHierarchy>()
                                 .set_priority(1001);


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////


struct TnaResetTransformMatrix
{
  void run(furious::Context* context, 
           uint32_t id, 
           TnaTransformMatrix*  matrix) 
  {
    matrix->m_dirty = false;
  }
};

furious::match<TnaTransformMatrix>().has_not_tag("__tna_static")
                                 .foreach<TnaResetTransformMatrix>()
                                 .set_post_frame();

END_FURIOUS_SCRIPT


