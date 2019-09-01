


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
create_entity(game_state_t* state)
{
  TnaEntity entity = furious::create_entity(state->p_database);
  FURIOUS_ADD_COMPONENT(entity,transform_t);
  FURIOUS_ADD_COMPONENT(entity,local_transform_matrix_t);
  FURIOUS_ADD_COMPONENT(entity,global_transform_matrix_t);

  return entity;
}

void 
remove_entity(TnaEntity entity)
{
  furious::destroy_entity(entity);
}

void
create_camera(game_state_t* state)
{
  fps_camera_t* camera = FURIOUS_CREATE_GLOBAL((state->p_database),
                                               fps_camera_t);

  fps_camera_init(camera, 
                  vector3_t(0.0f,0.0f,10.0f),
                  radians(0.0),
                  radians(0.0));

  camera->m_speed = 10.0f;
}

  
} /* tna */ 
