

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

struct GameState
{
  furious::Database* p_database;
  GLFWwindow*        p_window;
};

/**
 * \brief Creates an entity in the this game app
 *
 * \return The newly created entity
 */
Entity 
create_entity(GameState* game_state);

/**
 * \brief Removes an entity in this current game app
 *
 * \param entity The entity to be removed
 */
void 
remove_entity(Entity entity);

/**
 * \brief Creates the camera for the scene.
 */
void
create_camera(GameState* game_state);
  
} /* tna */ 



#endif /* ifndef _TNA_GAME_STATE_H_
*/
