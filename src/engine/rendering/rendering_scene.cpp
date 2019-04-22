

#include "rendering_scene.h"
#include "../shapes/aabb.h"
#include "mesh_data.h"
#include "../resources/resources.h"

#include "stdio.h"

namespace tna 
{

TnaRenderDescriptor*
get_descriptor(TnaRenderingScene* scene, TnaRenderHandler handler)
{
  if(scene->m_id_to_position[handler.m_id].m_mobility_type == TnaRenderMobilityType::E_STATIC)
  {
    uint32_t offset = scene->m_id_to_position[handler.m_id].m_offset;
    return &scene->m_static_meshes[offset];
  }
  
  uint32_t offset = scene->m_id_to_position[handler.m_id].m_offset;
  return &scene->m_dynamic_meshes[offset];
}

TnaRenderHandler
TnaRenderingScene::create_render_object(TnaRenderObjectType o_type,
                     TnaRenderMobilityType  mob_type)
{
  TnaRenderHandler handler;
  uint32_t offset = 0;
  switch(o_type)
  {
    case TnaRenderObjectType::E_MESH:
      // find descriptor slot based on mobility type
      TnaRenderDescriptor desc;
      desc.m_active = true;
      if(mob_type == TnaRenderMobilityType::E_STATIC)
      {
        if(m_static_gaps.size() > 0)
        {
          offset = m_static_gaps[m_static_gaps.size()-1];
          m_static_gaps.pop();
          m_static_meshes[offset] = desc;
        }
        else
        {
          offset = m_static_meshes.size();
          m_static_meshes.append(desc);
        }
      }
      else
      {
        if(m_dynamic_gaps.size() > 0)
        {
          offset = m_dynamic_gaps[m_dynamic_gaps.size()-1];
          m_dynamic_gaps.pop();
          m_dynamic_meshes[offset] = desc;
        }
        else
        {
          offset = m_dynamic_meshes.size();
          m_dynamic_meshes.append(desc);
        }
      }
      break;
  }

  // find gap to position index slot
  if(m_gaps_to_position.size() > 0)
  {
    handler.m_id = m_gaps_to_position[m_gaps_to_position.size()-1];
    m_gaps_to_position.pop();
    m_id_to_position[handler.m_id].m_offset = offset;
  }
  else
  {
    handler.m_id = m_id_to_position.size();
    m_id_to_position.append(TnaRenderIndex());
    m_id_to_position[handler.m_id].m_offset = offset;
  }

  m_id_to_position[handler.m_id].m_mobility_type = mob_type;
  return handler;
}

void
TnaRenderingScene::destroy_render_object(TnaRenderHandler handler)
{
  if(m_id_to_position[handler.m_id].m_mobility_type == TnaRenderMobilityType::E_STATIC)
  {
    uint32_t offset = m_id_to_position[handler.m_id].m_offset;
    m_static_meshes[offset] = TnaRenderDescriptor();
    m_static_gaps.append(offset);
  }
  else
  {
    uint32_t offset = m_id_to_position[handler.m_id].m_offset;
    m_dynamic_meshes[offset] = TnaRenderDescriptor();
    m_dynamic_gaps.append(offset);
  }
  m_gaps_to_position.append(handler.m_id);
}

void
TnaRenderingScene::set_mesh(TnaRenderHandler handler, 
                            const std::string& mesh)
{
  TnaRenderDescriptor* desc = get_descriptor(this, handler);
  desc->p_mesh_data = p_mesh_registry->load(mesh);
}

void
TnaRenderingScene::set_material(TnaRenderHandler handler, 
                                const TnaMaterialDescriptor& mat_desc)
{

  TnaRenderDescriptor* desc = get_descriptor(this, handler);
  desc->m_material = mat_desc;
}

void
TnaRenderingScene::get_material(TnaRenderHandler handler, 
                                TnaMaterialDescriptor* mat_desc)
{

  TnaRenderDescriptor* descriptor = get_descriptor(this, handler);
  *mat_desc = descriptor->m_material;
}

void
TnaRenderingScene::set_model_mat(TnaRenderHandler handler, 
                                 const TnaMatrix4& mat)
{
  TnaRenderDescriptor* desc = get_descriptor(this, handler);
  desc->m_placement.m_model_mat = mat;
}

void
TnaRenderingScene::set_frustrum_culling(TnaRenderHandler handler,
                                        const TnaMatrix4& proj_matrix)
{
  TnaRenderDescriptor* desc = get_descriptor(this, handler);
  if(desc->p_mesh_data == nullptr)
  {
    return;
  }

  const TnaAABB* aabb = &desc->p_mesh_data->m_aabb;
  TnaVector4 min = desc->m_placement.m_model_mat*TnaVector4(aabb->m_min,1.0f);
  TnaVector4 max = desc->m_placement.m_model_mat*TnaVector4(aabb->m_max,1.0f);
  TnaVector4 points[8] = {
    {min.x, min.y, min.z, 1.0f},
    {min.x, min.y, max.z, 1.0f},
    {min.x, max.y, min.z, 1.0f},
    {min.x, max.y, max.z, 1.0f},
    {max.x, min.y, min.z, 1.0f},
    {max.x, min.y, max.z, 1.0f},
    {max.x, max.y, min.z, 1.0f},
    {max.x, max.y, max.z, 1.0f}
  };
  bool visible = false;
  for(uint32_t i = 0; i < 8; ++i)
  {
    TnaVector4 proj_point = proj_matrix*points[i];
    if((-proj_point.w <= proj_point.x) && (proj_point.x <= proj_point.w) &&
       (-proj_point.w <= proj_point.y) && (proj_point.y <= proj_point.w) &&
       (0 <= proj_point.z) && (proj_point.z <= proj_point.w))
    {
      visible = true;
      break;
    }
  }
  desc->m_placement.m_frustrum_visible = visible;
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

} /* tna */ 
