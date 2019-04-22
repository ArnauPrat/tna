


#ifndef _TNA_ENGINE_H_
#define _TNA_ENGINE_H_ value

#include "game_app.h"
#include "common.h"
#include "rendering/rendering.h"
#include "resources/resources.h"

namespace tna 
{

class TnaConfig;
class TnaGameApp;

/**
 * \brief Initializes the resources of the game engine
 */
void 
initialize();

/**
 * \brief Releases the resources of the game engine
 */
void 
terminate();

/**
 * \brief Toggle the engine GUI
 */
void
toggle_gui();

/**
 * \brief Toggles the edit mode
 */
void
toggle_edit_mode();

/**
 * \brief Returns if in edit mode 
 */
bool
is_edit_mode();

/**
 * \brief Runs the given GameApp
 *
 * \param game_app
 */
void 
run(TnaGameApp* game_app);

}
#endif /* ifndef _TNA_ENGINE_H_ */
