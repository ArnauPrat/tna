


#include "components/fps_camera.h"
#include "components/look_at_camera.h"
#include "components/transform.h"
#include "components/transform_matrix.h"
#include "components/render_mesh_data.h"
#include "engine.h"
#include "game_state.h"
#include "math/math_tools.h"
#include "math/matrix.h"

#include <furious/furious.h>

namespace tna
{

TnaEntity 
create_entity(TnaGameState* state)
{
  TnaEntity entity = furious::create_entity(state->p_database);
  FURIOUS_ADD_COMPONENT(entity,TnaTransform);
  FURIOUS_ADD_COMPONENT(entity,TnaTransformMatrix);
  return entity;
}

void 
remove_entity(TnaEntity entity)
{
  furious::destroy_entity(entity);
}

void
create_camera(TnaGameState* state)
{
  TnaFPSCamera* camera = FURIOUS_CREATE_GLOBAL((state->p_database),
                                               TnaFPSCamera,
                                               TnaVector3(0.0f,0.0f,10.0f),
                                               radians(0.0),
                                               radians(0.0));
  camera->m_speed = 10.0f;
}

  
} /* tna */ 
