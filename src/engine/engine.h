


#ifndef _TNA_ENGINE_H_
#define _TNA_ENGINE_H_ value

#include "game_app.h"
#include "common.h"
#include "rendering/rendering.h"
#include "resources/resources.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <furious/furious.h>

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
