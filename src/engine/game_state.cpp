


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

/**
 * \brief Creates an entity in the this game app
 *
 * \return The newly created entity
 */
Entity 
create_entity(GameState* state)
{
  Entity entity = furious::create_entity(state->p_database);
  FURIOUS_ADD_COMPONENT((&entity),Transform);
  FURIOUS_ADD_COMPONENT((&entity),TransformMatrix);
  return entity;
}

/**
 * \brief Removes an entity in this current game app
 *
 * \param entity The entity to be removed
 */
void 
remove_entity(Entity entity)
{
  furious::destroy_entity(entity);
}

/**
 * \brief Creates the camera for the scene.
 */
void
create_camera(GameState* state)
{
  FPSCamera* camera = FURIOUS_CREATE_GLOBAL((state->p_database),
                                            FPSCamera,
                                            Vector3(0.0f,0.0f,10.0f),
                                            radians(0.0),
                                            radians(0.0));
  camera->m_speed = 10.0f;
}

  
} /* tna */ 
