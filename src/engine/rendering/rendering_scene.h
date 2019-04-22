

#ifndef _TNA_RENDERING_SCENE_H_
#define _TNA_RENDERING_SCENE_H_

#include "../math/matrix.h"
#include "../math/vector.h"
#include "renderer.h"
#include "furious/common/dyn_array.h"

namespace tna 
{

struct TnaMeshData;

struct TnaMaterialDescriptor
{
  TnaVector3   m_color;
};

struct TnaPlacementDescriptor
{
  TnaMatrix4   m_model_mat;
  bool      m_frustrum_visible;
};

struct TnaRenderMeshDescriptor
{
  const TnaMeshData*     p_mesh_data;
  TnaMaterialDescriptor  m_material;
  TnaPlacementDescriptor m_placement;
};


struct TnaRenderingScene 
{
  void 
  add_mesh(const TnaRenderMeshDescriptor& rmesh_desc);

  void 
  set_view_matrix(const TnaMatrix4& view_mat);

  void 
  set_proj_matrix(const TnaMatrix4& projection_mat);

  void
  set_clear_color(const TnaVector3& color);

  void 
  clear();

  void
  get_meshes(const TnaRenderMeshDescriptor** meshes, 
             uint32_t* num_meshes) const;


  furious::DynArray<TnaRenderMeshDescriptor> m_meshes;

  TnaMatrix4                                 m_view_mat;
  TnaMatrix4                                 m_proj_mat;
  TnaVector3                                 m_clear_color;
};
  
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
