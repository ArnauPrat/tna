

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

enum class TnaRenderMobilityType
{
  E_STATIC,
  E_DYNAMIC
};

enum class TnaRenderObjectType
{
  E_MESH,
};

struct TnaMaterialDescriptor
{
  TnaVector3   m_color = TNA_COLOR_WHITE;
};

struct TnaPlaceDescriptor
{
  TnaMatrix4      m_model_mat         = TnaMatrix4(1.0f);
  bool            m_frustrum_visible  = true;
};

struct TnaRenderDescriptor
{
  bool                    m_active             = false;
  TnaRenderObjectType     m_render_object_type = TnaRenderObjectType::E_MESH;
  const TnaMeshData*      p_mesh_data          = nullptr;
  TnaMaterialDescriptor   m_material;
  TnaPlaceDescriptor      m_placement;
};

struct TnaRenderHandler
{
  uint32_t m_id;
};

struct TnaRenderIndex
{
  uint32_t        m_offset = 0;
  TnaRenderMobilityType m_mobility_type = TnaRenderMobilityType::E_DYNAMIC;
};

TnaRenderDescriptor*
get_descriptor(TnaRenderingScene* scene, TnaRenderHandler);

struct TnaRenderingScene 
{

  TnaRenderHandler
  create_render_object(TnaRenderObjectType o_type,
                       TnaRenderMobilityType  m_type);

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


  furious::DynArray<TnaRenderIndex>           m_id_to_position;
  furious::DynArray<uint32_t>                 m_gaps_to_position;

  furious::DynArray<TnaRenderDescriptor>      m_static_meshes;
  furious::DynArray<uint32_t>                 m_static_gaps;

  furious::DynArray<TnaRenderDescriptor> m_dynamic_meshes;
  furious::DynArray<uint32_t>                 m_dynamic_gaps;

  TnaMatrix4                                  m_view_mat;
  TnaMatrix4                                  m_proj_mat;
  TnaVector3                                  m_clear_color;
};
  
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
