
#include "../engine/engine.h"
#include "../game_references.h"
#include "../engine/math/matrix.h"
#include "../engine/math/math_tools.h"
#include "../engine/components/transform.h"
#include "../components/rotation_speed.h"

#include <furious/lang/lang.h>
#include "stdio.h"

BEGIN_FURIOUS_SCRIPT

using namespace tna;

struct Follower
{
  Follower()  
  {
  }
  
  void run(furious::Context* context, 
           uint32_t id, 
           TnaTransform* transform) 
  {
  }
};

furious::match<TnaTransform>().expand<>(TNA_GAME_REF_IS_LEAD_BY)
                                                   .foreach<Follower>();

END_FURIOUS_SCRIPT
