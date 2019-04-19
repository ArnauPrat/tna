

#ifndef _TNA_RENDERER_H_
#define _TNA_RENDERER_H_ value

#include "../math/matrix.h"
#include "../math/vector.h"

class GLFWwindow;

namespace tna 
{

class Config;
struct MeshData;

struct MaterialDescriptor
{
  Vector3   m_color;
};

struct PlacementDescriptor
{
  Matrix4   m_model_mat;
  bool      m_frustrum_visible;
};

struct RenderMeshDescriptor
{
  const MeshData*     p_mesh_data;
  MaterialDescriptor  m_material;
  PlacementDescriptor m_placement;
};

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
 * @param rmesh_desc The render mesh descriptor to render
 */
void 
render_mesh(const RenderMeshDescriptor& rdesc);


/**
 * @brief Sets the camera of the scene at the given position represented the
 * given matrix.
 *
 * @param view_mat The view matrix
 */
void 
set_view_matrix(const Matrix4& view_mat);

/**
 * \brief Sets the projection matrix of the renderer
 *
 * \param proj_mat The projection matrix
 */
void
set_proj_matrix(const Matrix4& proj_mat);

/**
 * \brief Sets the clear color 
 *
 * \param color The color to set
 */
void
set_clear_color(const Vector3& color);

}

#endif /* ifndef _TNA_RENDERING_H_ */
