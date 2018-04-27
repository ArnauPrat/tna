

#ifndef _TNA_RENDERING_H_
#define _TNA_RENDERING_H_ value

#include <vulkan/vulkan.h>

class GLFWwindow;

namespace tna {

class Config;

/**
 * @brief Vulkan Instance
 */
extern VkInstance  vulkan_instance;

/**
 * @brief Initializes the rendering subsystem
 *
 * @param config
 */
void init_rendering(const Config& config, GLFWwindow* window);

/**
 * @brief Terminates the rendering subsystem
 */
void terminate_rendering();


/**
 * @brief Draws a frame
 */
void draw_frame();

}


#endif /* ifndef _TNA_RENDERING_H_ */
