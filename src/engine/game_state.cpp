


#include "components/fps_camera.h"
#include "components/look_at_camera.h"
#include "components/transform.h"
#include "components/transform_matrix.h"
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
set_static(TnaEntity entity)
{
  FURIOUS_ADD_TAG(entity, "__tna_static");
  const TnaTransform* transform = FURIOUS_GET_COMPONENT(entity, TnaTransform);
  TnaTransformMatrix* transform_matrix = FURIOUS_GET_COMPONENT(entity, TnaTransformMatrix);
  transform_matrix->m_matrix = transform->to_matrix();
  transform_matrix->m_dirty = false;
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
