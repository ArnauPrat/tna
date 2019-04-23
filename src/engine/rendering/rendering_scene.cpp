

#include "rendering_scene.h"
#include "../shapes/aabb.h"
#include "mesh_data.h"
#include "../resources/resources.h"

#include "stdio.h"

namespace tna 
{

TnaRenderingScene::TnaRenderingScene() :
m_static_uniforms(nullptr),
m_dynamic_uniforms(nullptr)
{
  TnaDeviceProperties device_properties = get_device_properties();
  size_t uniform_data_size = sizeof(TnaRenderMeshUniform);
  m_uniform_alignment = (uniform_data_size / device_properties.m_uniform_offset_alignment) * device_properties.m_uniform_offset_alignment + ((uniform_data_size % device_properties.m_uniform_offset_alignment) > 0 ? device_properties.m_uniform_offset_alignment : 0);
  m_static_uniforms = new char[m_uniform_alignment*MAX_PRIMITIVE_COUNT];
  m_dynamic_uniforms = new char[m_uniform_alignment*MAX_PRIMITIVE_COUNT];
}

TnaRenderingScene::~TnaRenderingScene()
{
  if(m_static_uniforms != nullptr)
  {
    delete [] m_static_uniforms;
  }

  if(m_dynamic_uniforms != nullptr)
  {
    delete [] m_dynamic_uniforms;
  }
}

TnaRenderHandler
TnaRenderingScene::create_render_object(TnaRenderObjectType o_type,
                     TnaRenderMobilityType  mob_type)
{
  TnaRenderHandler handler;
  TnaRenderHeader* header;
  if(m_header_gaps.size() > 0)
  {
    handler.m_id = m_header_gaps[m_header_gaps.size()-1];
    m_header_gaps.pop();
    header = &m_headers[handler.m_id];
  }
  else
  {
    handler.m_id = m_headers.size();
    m_headers.append(TnaRenderHeader());
    header = &m_headers[handler.m_id];
  }

  header->m_active = true;
  header->m_frustrum_visible = true;
  header->m_render_object_type = o_type;
  header->m_mobility_type = mob_type;
  switch(header->m_render_object_type)
  {
    case TnaRenderObjectType::E_MESH:
      if(header->m_mobility_type == TnaRenderMobilityType::E_STATIC)
      {
        if(m_static_gaps.size() > 0)
        {
          header->m_offset = m_static_gaps[m_static_gaps.size()-1];
          m_static_gaps.pop();
          m_static_meshes[header->m_offset] = nullptr;
        }
        else
        {
          header->m_offset = m_static_meshes.size();
          m_static_meshes.append(nullptr);
          *((TnaRenderMeshUniform*)&m_static_uniforms[m_uniform_alignment*header->m_offset]) = TnaRenderMeshUniform();
        }
      }
      else
      {
        if(m_dynamic_gaps.size() > 0)
        {
          header->m_offset = m_dynamic_gaps[m_dynamic_gaps.size()-1];
          m_dynamic_gaps.pop();
          m_dynamic_meshes[header->m_offset] = nullptr;
        }
        else
        {
          header->m_offset = m_dynamic_meshes.size();
          *((TnaRenderMeshUniform*)&m_dynamic_uniforms[m_uniform_alignment*header->m_offset]) = TnaRenderMeshUniform();
          m_dynamic_meshes.append(nullptr);
        }
      }
      break;
  }
  return handler;
}

void
TnaRenderingScene::destroy_render_object(TnaRenderHandler handler)
{
  TnaRenderHeader* header = &m_headers[handler.m_id];
  uint32_t offset = header->m_offset;
  if(header->m_mobility_type == TnaRenderMobilityType::E_STATIC)
  {
    m_static_gaps.append(offset);
  }
  else
  {
    m_dynamic_gaps.append(offset);
  }
  header->m_active = false;
  m_header_gaps.append(handler.m_id);
}

void
TnaRenderingScene::set_mesh(TnaRenderHandler handler, 
                            const std::string& mesh)
{
  TnaRenderHeader* header = &m_headers[handler.m_id];
  if(header->m_mobility_type == TnaRenderMobilityType::E_STATIC)
  {
    m_static_meshes[header->m_offset] = p_mesh_registry->load(mesh);
  }
  else
  {
    m_dynamic_meshes[header->m_offset] = p_mesh_registry->load(mesh);
  }
}

void
TnaRenderingScene::set_material(TnaRenderHandler handler, 
                                const TnaMaterialDescriptor& mat_desc)
{
  TnaRenderHeader* header = &m_headers[handler.m_id];
  TnaRenderMeshUniform* uniform = nullptr;
  if(header->m_mobility_type == TnaRenderMobilityType::E_STATIC)
  {
    uniform = ((TnaRenderMeshUniform*)&m_static_uniforms[m_uniform_alignment*header->m_offset]);
  }
  else
  {
    uniform = ((TnaRenderMeshUniform*)&m_dynamic_uniforms[m_uniform_alignment*header->m_offset]);
  }
  uniform->m_material = mat_desc;
}

void
TnaRenderingScene::get_material(TnaRenderHandler handler, 
                                TnaMaterialDescriptor* mat_desc)
{

  TnaRenderHeader* header = &m_headers[handler.m_id];
  TnaRenderMeshUniform* uniform = nullptr;
  if(header->m_mobility_type == TnaRenderMobilityType::E_STATIC)
  {
    uniform = ((TnaRenderMeshUniform*)&m_static_uniforms[m_uniform_alignment*header->m_offset]);
  }
  else
  {
    uniform = ((TnaRenderMeshUniform*)&m_dynamic_uniforms[m_uniform_alignment*header->m_offset]);
  }
  *mat_desc = uniform->m_material;
}

void
TnaRenderingScene::set_model_mat(TnaRenderHandler handler, 
                                 const TnaMatrix4& mat)
{
  TnaRenderHeader* header = &m_headers[handler.m_id];
  TnaRenderMeshUniform* uniform = nullptr;
  if(header->m_mobility_type == TnaRenderMobilityType::E_STATIC)
  {
    uniform = ((TnaRenderMeshUniform*)&m_static_uniforms[m_uniform_alignment*header->m_offset]);
  }
  else
  {
    uniform = ((TnaRenderMeshUniform*)&m_dynamic_uniforms[m_uniform_alignment*header->m_offset]);
  }
  uniform->m_model_matrix = mat;
}

void
TnaRenderingScene::set_frustrum_culling(TnaRenderHandler handler,
                                        const TnaMatrix4& proj_matrix)
{
  TnaRenderHeader* header = &m_headers[handler.m_id];
  TnaMeshData* mesh_data = nullptr;
  TnaRenderMeshUniform* uniform = nullptr;
  if(header->m_mobility_type == TnaRenderMobilityType::E_STATIC)
  {
    mesh_data = m_static_meshes[header->m_offset];
    uniform = ((TnaRenderMeshUniform*)&m_static_uniforms[m_uniform_alignment*header->m_offset]);
  }
  else
  {
    mesh_data = m_dynamic_meshes[header->m_offset];
    uniform = ((TnaRenderMeshUniform*)&m_dynamic_uniforms[m_uniform_alignment*header->m_offset]);
  }

  if(mesh_data == nullptr)
  {
    return;
  }

  const TnaAABB* aabb = &mesh_data->m_aabb;
  TnaVector4 min = uniform->m_model_matrix*TnaVector4(aabb->m_min,1.0f);
  TnaVector4 max = uniform->m_model_matrix*TnaVector4(aabb->m_max,1.0f);
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
  header->m_frustrum_visible = visible;
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
