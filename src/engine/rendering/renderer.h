

#ifndef _TNA_RENDERER_H_
#define _TNA_RENDERER_H_ value

#include "../math/matrix.h"
#include "../math/vector.h"

class GLFWwindow;

namespace tna 
{

class Config;
struct MeshData;
struct RenderingScene;

extern RenderingScene* p_rendering_scene;

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
begin_frame(RenderingScene* scene);

/**
 * @brief Draws a frame
 */
void
end_frame(RenderingScene* scene);

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
