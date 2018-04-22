


#ifndef _TNA_ENGINE_H_
#define _TNA_ENGINE_H_ value

#include "game_app.h"
#include "common.h"
#include "glm/glm.hpp"

namespace tna {

class Config;
class GameApp;

/**
 * @brief Initializes the resources of the game engine
 */
void initialize();

/**
 * @brief Releases the resources of the game engine
 */
void terminate();

/**
 * @brief Runs the given GameApp
 *
 * @param game_app
 */
void run(GameApp* game_app);

}
#endif /* ifndef _TNA_ENGINE_H_ */
