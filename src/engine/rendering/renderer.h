

#ifndef _TNA_RENDERER_H_
#define _TNA_RENDERER_H_ value

#include "../math/matrix.h"
#include "../math/vector.h"

class GLFWwindow;

namespace tna 
{

#define MAX_PRIMITIVE_COUNT 1000000
#define MAX_FRAME_BUFFERS 3

struct config_t;
struct rendering_scene_t;
extern rendering_scene_t* p_rendering_scene;

struct device_properties_t 
{
  size_t m_uniform_offset_alignment;
};


/**
 * \brief Creates the rendering scene structure
 */
void
rendering_scene_create();

/**
 * \brief Destroys the rendering scene structure
 */
void
rendering_scene_destroy();

/**
 * @brief Initializes the rendering subsystem
 *
 * @param config
 */
void
init_renderer(const config_t* config, 
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
begin_frame(rendering_scene_t* scene);

/**
 * @brief Draws a frame
 */
void
end_frame(rendering_scene_t* scene);

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

/**
 * \brief Gets the device properties of the particular renderer implementation
 *
 * \return Returns a TnaDeviceProperties structure with the property values
 */
device_properties_t
get_device_properties();

}

#endif /* ifndef _TNA_RENDERING_H_ */
