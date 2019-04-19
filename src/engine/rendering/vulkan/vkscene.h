

#ifndef _TNA_VKSCENE_H_
#define _TNA_VKSCENE_H_

#include "../../math/matrix.h"
#include "../../math/vector.h"
#include "furious/common/dyn_array.h"
#include "../renderer.h"

namespace tna 
{

struct VkMeshData;

struct VkScene 
{
  VkScene() = default;
  virtual ~VkScene() = default;

  void 
  add_mesh(const RenderMeshDescriptor& rmesh_desc);

  void 
  set_view_matrix(const Matrix4& view_mat);

  void 
  set_proj_matrix(const Matrix4& projection_mat);

  void
  set_clear_color(const Vector3& color);

  void 
  clear();

  void
  get_meshes(const RenderMeshDescriptor** meshes, 
             uint32_t* num_meshes) const;


  furious::DynArray<RenderMeshDescriptor> m_meshes;

  Matrix4                                 m_view_mat;
  Matrix4                                 m_proj_mat;
  Vector3                                 m_clear_color;

};
  
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
