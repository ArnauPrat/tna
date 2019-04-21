
#include "../engine.h"
#include "../math/matrix.h"
#include "../math/math_tools.h"
#include "../components/transform.h"
#include "../components/transform_matrix.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct InitTransformMatrix 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TransformMatrix* model_matrix,
           Transform* transform) 
  {
    if(transform->m_dirty)
    {
      model_matrix->m_matrix = transform->to_matrix();
      model_matrix->m_dirty = true;
      transform->m_dirty = false;
    }
  }
};

furious::match<TransformMatrix, Transform>().has_not_tag("static")
                                            .foreach<InitTransformMatrix>()
                                            .set_priority(1000);

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

struct TransformMatrixHierarchy 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TransformMatrix* model_matrix,
           const TransformMatrix* parent_matrix) 
  {
    if(parent_matrix->m_dirty || model_matrix->m_dirty)
    {
      model_matrix->m_matrix = parent_matrix->m_matrix * model_matrix->m_matrix;
      model_matrix->m_dirty = true;
    }
  }
};


furious::match<TransformMatrix>().expand<TransformMatrix>("parent")
                                 .foreach<TransformMatrixHierarchy>()
                                 .set_priority(1001);


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////


/*struct ResetTransform 
{
  void run(furious::Context* context, 
           uint32_t id, 
           Transform*  transform) 
  {
    transform->m_dirty = false;
  }
};

furious::match<Transform>().foreach<ResetTransform>()
                           .set_post_frame();
                           */

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////


struct ResetTransformMatrix
{
  void run(furious::Context* context, 
           uint32_t id, 
           TransformMatrix*  matrix) 
  {
    matrix->m_dirty = false;
  }
};

furious::match<TransformMatrix>().has_not_tag("static")
                                 .foreach<ResetTransformMatrix>()
                                 .set_post_frame();

END_FURIOUS_SCRIPT


