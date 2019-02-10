

#ifndef _TNA_VKSCENE_H_
#define _TNA_VKSCENE_H_ value

#include "../../math/matrix.h"
#include <vector>

namespace tna 
{

struct VkMeshData;

struct VkRenderingInfo 
{
  VkRenderingInfo(const VkMeshData* mesh_data,
                  const Matrix4*  model_mat);
  const VkMeshData* m_mesh_data;
  const Matrix4     m_model_mat;
};

struct VkScene 
{
  VkScene() = default;
  virtual ~VkScene() = default;

  void 
  add_mesh(const VkMeshData* mesh,
           const Matrix4* model_mat);

  void 
  set_camera(const Matrix4* camera_mat);

  void 
  clear();

  void
  get_meshes(const VkRenderingInfo** meshes, 
             uint32_t* num_meshes) const;

  Matrix4 get_camera() const;

private:

  std::vector<VkRenderingInfo> m_meshes;

  Matrix4                      m_camera;

};
  
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
