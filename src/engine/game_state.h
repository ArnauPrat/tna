

#ifndef _TNA_GAME_STATE_H_
#define _TNA_GAME_STATE_H_

#include "common.h"
#include "entity.h"

struct GLFWwindow;

namespace furious
{
struct Database;
} 

namespace tna
{

struct TnaGameState
{
  furious::Database* p_database;
  GLFWwindow*        p_window;
};

/**
 * \brief Creates an entity in this game app
 *
 * \return The newly created entity
 */
TnaEntity 
create_entity(TnaGameState* game_state);

/**
 * \brief Removes an entity in this current game app
 *
 * \param entity The entity to be removed
 */
void 
remove_entity(TnaEntity entity);

/**
 * \brief Sets the entity as an static entity. Changes to transform in static
 * entities won't be propagated to the renderer
 *
 * \param entity The entity to set as static
 */
void
set_static(TnaEntity entity);

/**
 * \brief Creates the camera for the scene.
 */
void
create_camera(TnaGameState* game_state);
  
} /* tna */ 



#endif /* ifndef _TNA_GAME_STATE_H_
*/
