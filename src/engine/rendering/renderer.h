

#ifndef _TNA_RENDERER_H_
#define _TNA_RENDERER_H_ value

#include "mesh_data.h"
#include <glm/glm.hpp>

class GLFWwindow;

namespace tna 
{

class Config;

/**
 * @brief Initializes the rendering subsystem
 *
 * @param config
 */
void
init_renderer(const Config* config, 
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
begin_frame();

/**
 * @brief Draws a frame
 */
void
end_frame();

/**
 * @brief Renders a mesh with tht given model matrix
 *
 * @param mesh_data The mesh to render
 * @param model_mat The model matrix
 */
void 
render_mesh(const MeshData* mesh_data, 
            const glm::mat4* model_mat );


/**
 * @brief Sets the camera of the scene at the given position represented the
 * given matrix.
 *
 * @param camera_mat The camera matrix to set the camera to
 */
void 
set_camera(const glm::mat4* camera_mat);

}

#endif /* ifndef _TNA_RENDERING_H_ */
