

#ifndef _TNA_RENDERER_H_
#define _TNA_RENDERER_H_ value

#include <vulkan/vulkan.h>
#include "../resources/resource_registry.h"
#include "shader.h"

class GLFWwindow;

namespace tna {

class Config;

/**
 * @brief Vulkan Instance
 */
namespace rendering {

/**
 * @brief Initializes the rendering subsystem
 *
 * @param config
 */
void init_renderer(const Config& config, GLFWwindow* window);

/**
 * @brief Terminates the rendering subsystem
 */
void terminate_renderer();


/**
 * @brief Draws a frame
 */
void draw_frame();


}

}


#endif /* ifndef _TNA_RENDERING_H_ */
