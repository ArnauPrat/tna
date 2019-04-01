

#ifndef _TNA_RENDERER_H_
#define _TNA_RENDERER_H_ value

#include "mesh_data.h"
#include "../math/matrix.h"
#include "../math/vector.h"

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
            const Matrix4* model_mat,
            const Vector3* color);


/**
 * @brief Sets the camera of the scene at the given position represented the
 * given matrix.
 *
 * @param camera_mat The camera matrix to set the camera to
 */
void 
set_camera(const Matrix4* camera_mat);

/**
 * \brief Sets the clear color 
 *
 * \param color The color to set
 */
void
set_clear_color(const Vector3* color);

}

#endif /* ifndef _TNA_RENDERING_H_ */
