

#include "rendering_scene.h"

namespace tna 
{

void 
TnaRenderingScene::add_mesh(const TnaRenderMeshDescriptor& rmesh_desc) 
{
  m_meshes.append(rmesh_desc);
}

void 
TnaRenderingScene::clear() 
{
  m_meshes.reset();
}

void 
TnaRenderingScene::set_view_matrix(const TnaMatrix4& view_mat) 
{
  m_view_mat = view_mat;
}

void 
TnaRenderingScene::set_proj_matrix(const TnaMatrix4& proj_mat) 
{
  m_proj_mat = proj_mat;
}

void
TnaRenderingScene::set_clear_color(const TnaVector3& color)
{
  m_clear_color = color;
}

void
TnaRenderingScene::get_meshes(const TnaRenderMeshDescriptor** meshes, 
                    uint32_t* num_meshes) const 
{
  *meshes = m_meshes.buffer();
  *num_meshes = m_meshes.size();
}
  
} /* tna */ 
