

#ifndef _TNA_RENDERER_H_
#define _TNA_RENDERER_H_ value

#include "../math/matrix.h"
#include "../math/vector.h"

class GLFWwindow;

namespace tna 
{

#define MAX_FRAME_BUFFERS 3

class TnaConfig;
struct TnaRenderingScene;

extern TnaRenderingScene* p_rendering_scene;

/**
 * \brief Creates the rendering scene structure
 */
void
create_rendering_scene();

/**
 * \brief Destroys the rendering scene structure
 */
void
destroy_rendering_scene();

/**
 * @brief Initializes the rendering subsystem
 *
 * @param config
 */
void
init_renderer(const TnaConfig* config, 
              GLFWwindow* window);

/**
 * @brief Terminates the rendering subsystem
 */
void
terminate_renderer();

/**
 * @brief Frees resources and starts the execution of a frame
 */
void
begin_frame(TnaRenderingScene* scene);

/**
 * @brief Draws a frame
 */
void
end_frame(TnaRenderingScene* scene);

/**
 * \brief Initializes the gui subsystem
 */
void
init_gui();

/**
 * \brief Terminates the gui subsystem
 */
void
terminate_gui();

}

#endif /* ifndef _TNA_RENDERING_H_ */
