

#ifndef _TNA_RENDERING_SCENE_H_
#define _TNA_RENDERING_SCENE_H_

#include "../math/matrix.h"
#include "../math/vector.h"
#include "../tools/colors.h"

#include "renderer.h"
#include "furious/common/dyn_array.h"

#include <string>

namespace tna 
{

struct TnaMeshData;

enum class TnaRenderObjectType
{
  E_MESH,
};

struct TnaMaterialDescriptor
{
  TnaVector3   m_color = TNA_COLOR_WHITE;
};

struct TnaRenderMeshUniform
{
  TnaMatrix4            m_model_matrix;
  TnaMaterialDescriptor m_material;
};

struct TnaRenderHandler
{
  uint32_t m_id;
};

struct TnaRenderHeader
{
  uint32_t                m_offset              = 0;
  TnaRenderObjectType     m_render_object_type  = TnaRenderObjectType::E_MESH;
  bool                    m_active              = false;
  bool                    m_frustrum_visible    = true;
};

struct TnaRenderingScene 
{
  TnaRenderingScene();
  ~TnaRenderingScene();

  TnaRenderHandler
  create_render_object(TnaRenderObjectType o_type);

  void
  destroy_render_object(TnaRenderHandler handler);


  void
  set_mesh(TnaRenderHandler handler, 
           const std::string& mesh);

  void
  set_material(TnaRenderHandler handler, 
               const TnaMaterialDescriptor& mat_desc);

  void
  get_material(TnaRenderHandler handler,
               TnaMaterialDescriptor* mat_desc);

  void
  set_model_mat(TnaRenderHandler handler, 
                  const TnaMatrix4& mat);

  void
  set_frustrum_culling(TnaRenderHandler handler,
                      const TnaMatrix4& proj_matrix);

  void 
  set_view_matrix(const TnaMatrix4& view_mat);

  void 
  set_proj_matrix(const TnaMatrix4& projection_mat);

  void
  set_clear_color(const TnaVector3& color);

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  ////////////////////////////////////////////////


  furious::DynArray<TnaRenderHeader>          m_headers;
  furious::DynArray<uint32_t>                 m_header_gaps;

  char*                                       m_uniforms;
  furious::DynArray<TnaMeshData*>             m_meshes;
  furious::DynArray<uint32_t>                 m_gaps;

  TnaMatrix4                                  m_view_mat;
  TnaMatrix4                                  m_proj_mat;
  TnaVector3                                  m_clear_color;

  size_t                                      m_uniform_alignment;
};
  
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
