

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

struct game_state_t 
{
  furious::Database* p_database = nullptr;
  GLFWwindow*        p_window   = nullptr;
};

/**
 * \brief Creates an entity in this game app
 *
 * \return The newly created entity
 */
TnaEntity 
create_entity(game_state_t* game_state);

/**
 * \brief Removes an entity in this current game app
 *
 * \param entity The entity to be removed
 */
void 
remove_entity(TnaEntity entity);

/**
 * \brief Creates the camera for the scene.
 */
void
create_camera(game_state_t* game_state);
  
} /* tna */ 



#endif /* ifndef _TNA_GAME_STATE_H_
*/
