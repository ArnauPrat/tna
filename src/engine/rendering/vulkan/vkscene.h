

#ifndef _TNA_VKSCENE_H_
#define _TNA_VKSCENE_H_ value

#include <glm/glm.hpp>
#include <vector>

namespace tna {
namespace rendering {

struct VkMeshData;

struct VkRenderingInfo {
  VkRenderingInfo(const VkMeshData* mesh_data,
                  const glm::mat4&  model_mat);
  const VkMeshData* m_mesh_data;
  const glm::mat4   m_model_mat;
};

class VkScene {

public:
  VkScene() = default;
  virtual ~VkScene() = default;

  void add_mesh(const VkMeshData* mesh,
                const glm::mat4& model_mat);

  void set_camera(const glm::mat4& camera_mat);

  void clear();

  const std::vector<VkRenderingInfo>& get_meshes() const;

  const glm::mat4& get_camera() const;

private:

  std::vector<VkRenderingInfo> m_meshes;

  glm::mat4                    m_camera;

};
  
} /* rendering */ 
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
