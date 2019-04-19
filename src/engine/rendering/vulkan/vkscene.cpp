

#include "vkscene.h"

namespace tna {

void 
VkScene::add_mesh(const RenderMeshDescriptor& rmesh_desc) 
{
  m_meshes.append(rmesh_desc);
}

void 
VkScene::clear() 
{
  m_meshes.reset();
}

void 
VkScene::set_view_matrix(const Matrix4& view_mat) 
{
  m_view_mat = view_mat;
}

void 
VkScene::set_proj_matrix(const Matrix4& proj_mat) 
{
  m_proj_mat = proj_mat;
}

void
VkScene::set_clear_color(const Vector3& color)
{
  m_clear_color = color;
}

void
VkScene::get_meshes(const RenderMeshDescriptor** meshes, 
                    uint32_t* num_meshes) const 
{
  *meshes = m_meshes.buffer();
  *num_meshes = m_meshes.size();
}
  
} /* tna */ 
