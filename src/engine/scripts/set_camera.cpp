
#include "../engine.h"
#include "../components/transform_matrix.h"

#include <furious/lang/lang.h>

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct SetCamera 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const TransformMatrix* transform_matrix) 
  {
    Matrix4 inv = inverse(transform_matrix->m_matrix);
    set_camera(&inv);
  }
};

furious::match<TransformMatrix>().has_tag("MainCamera").foreach<SetCamera>();

END_FURIOUS_SCRIPT
