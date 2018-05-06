

#include "vkscene.h"

namespace tna {
namespace rendering {

VkRenderingInfo::VkRenderingInfo(const VkMeshData* mesh_data,
                                 const glm::mat4&  model_mat) :
m_mesh_data{mesh_data},
m_model_mat{model_mat} {

}

void VkScene::add_mesh(VkMeshData* mesh,
                              const glm::mat4& model_mat) {
  m_meshes.emplace_back(mesh, model_mat);
}

void VkScene::clear() {
  m_meshes.clear();
}

void VkScene::set_camera(const glm::mat4& camera_mat) {
  m_camera = camera_mat;

}

const std::vector<VkRenderingInfo>& VkScene::get_meshes() const {
  return m_meshes;
}

const glm::mat4& VkScene::get_camera() const {
  return m_camera;
}
  
} /* rendering */ 
} /* tna */ 
