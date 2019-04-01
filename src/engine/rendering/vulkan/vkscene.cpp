

#include "vkscene.h"

namespace tna {

VkRenderingInfo::VkRenderingInfo(const VkMeshData* mesh_data,
                                 const Matrix4*  model_mat,
                                 const Vector3*   color) :
m_mesh_data(mesh_data),
m_model_mat(*model_mat),
m_color(*color)
{

}

void 
VkScene::add_mesh(const VkMeshData* mesh,
                  const Matrix4* model_mat,
                  const Vector3* color) 
{
  m_meshes.emplace_back(mesh, model_mat, color);
}

void 
VkScene::clear() 
{
  m_meshes.clear();
}

void 
VkScene::set_camera(const Matrix4* camera_mat) 
{
  m_camera = *camera_mat;
}

void
VkScene::set_clear_color(const Vector3* color)
{
  m_clear_color = *color;
}

void
VkScene::get_meshes(const VkRenderingInfo** meshes, 
                    uint32_t* num_meshes) const 
{
  *meshes = m_meshes.data();
  *num_meshes = m_meshes.size();
}
  
} /* tna */ 
