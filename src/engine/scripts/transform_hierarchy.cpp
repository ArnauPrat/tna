
#include "../engine.h"
#include "../tools/references.h"
#include "../math/matrix.h"
#include "../math/math_tools.h"
#include "../components/transform.h"
#include "../components/transform_matrix.h"
#include "priorities.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct TnaInitTransformMatrix 
{
  void run(furious::Context* context, 
           uint32_t id, 
           local_transform_matrix_t* local_matrix,
           global_transform_matrix_t* global_matrix,
           transform_t* transform) 
  {
    local_matrix->m_matrix = transform_to_matrix(transform);
    local_matrix->m_dirty = true;
    global_matrix->m_matrix = local_matrix->m_matrix;
    transform->m_dirty = false;
  }
};

furious::match<local_transform_matrix_t, global_transform_matrix_t, transform_t>().filter([](const local_transform_matrix_t* local_matrix,
                                                                                            const global_transform_matrix_t* global_matrix,
                                                                                            const transform_t* transform)
                                                    {
                                                      return transform->m_dirty;
                                                    }
                                                    )
                                            .foreach<TnaInitTransformMatrix>()
                                            .set_priority(PRIORITY_INIT_TRANSFORM_MATRIX);

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

struct TnaTransformMatrixHierarchy 
{
  void run(furious::Context* context, 
           uint32_t id, 
           global_transform_matrix_t* global_matrix,
           const local_transform_matrix_t* local_matrix,
           const local_transform_matrix_t* parent_matrix) 
  {
    global_matrix->m_matrix = parent_matrix->m_matrix * local_matrix->m_matrix;
  }
};


furious::match<global_transform_matrix_t, local_transform_matrix_t>().expand<local_transform_matrix_t>(TNA_REF_PARENT)
                                    .filter([] (const global_transform_matrix_t* global_matrix,
                                                const local_transform_matrix_t* local_matrix,
                                                const local_transform_matrix_t* parent_matrix)
                                         {
                                          return local_matrix->m_dirty || parent_matrix->m_dirty;
                                         }
                                         )
                                    .foreach<TnaTransformMatrixHierarchy>()
                                    .set_priority(PRIORITY_TRANSFORM_MATRIX_HIERARCHY);


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////


struct TnaResetLocalTransformMatrix
{
  void run(furious::Context* context, 
           uint32_t id, 
           local_transform_matrix_t*  local_matrix) 
  {
    local_matrix->m_dirty = false;
  }
};

furious::match<local_transform_matrix_t>().foreach<TnaResetLocalTransformMatrix>()
                                         .set_post_frame();

END_FURIOUS_SCRIPT


