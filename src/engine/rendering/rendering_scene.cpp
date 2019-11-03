

#include "rendering_scene.h"
#include "../shapes/aabb.h"
#include "mesh_data.h"
#include "../resources/resources.h"

#include "stdio.h"
#include "assert.h"

namespace tna 
{

void
rendering_scene_init(rendering_scene_t* scene)
{
  scene->m_uniforms = new render_mesh_uniform_t[MAX_PRIMITIVE_COUNT];
}

void
rendering_scene_release(rendering_scene_t* scene)
{
  if(scene->m_uniforms != nullptr)
  {
    delete [] scene->m_uniforms;
    scene->m_uniforms = nullptr;
  }
}

render_handler_t
rendering_scene_create_object(rendering_scene_t* scene,
                              render_object_type_t o_type)
{
  render_handler_t handler;
  render_header_t* header;
  if(scene->m_header_gaps.size() > 0)
  {
    handler = scene->m_header_gaps[scene->m_header_gaps.size()-1];
    scene->m_header_gaps.pop();
    header = &scene->m_headers[handler];
  }
  else
  {
    handler = scene->m_headers.size();
    scene->m_headers.append(render_header_t());
    header = &scene->m_headers[handler];
  }

  header->m_active = true;
  header->m_frustrum_visible = true;
  header->m_render_object_type = o_type;
  switch(header->m_render_object_type)
  {
    case render_object_type_t::E_MESH:
      if(scene->m_gaps.size() > 0)
      {
        header->m_offset = scene->m_gaps[scene->m_gaps.size()-1];
        scene->m_gaps.pop();
        scene->m_meshes[header->m_offset] = nullptr;
      }
      else
      {
        header->m_offset = scene->m_meshes.size();
        if(header->m_offset >= MAX_PRIMITIVE_COUNT)
        {
          TNA_LOG_ERROR(TNA_ERROR::E_RENDERER_RUNTIME_ERROR, 
                        "Exceeded max primitive count");
        }
        scene->m_meshes.append(nullptr);
        *((render_mesh_uniform_t*)&scene->m_uniforms[header->m_offset]) = render_mesh_uniform_t();
      }
      break;
  }
  return handler;
}

void
rendering_scene_destroy_object(rendering_scene_t* scene,
                               render_handler_t& handler)
{
  assert(handler != _TNA_RENDER_HANDLER_INVALID && "Invalid render handler");

  render_header_t* header = &scene->m_headers[handler];
  uint32_t offset = header->m_offset;
  scene->m_gaps.append(offset);
  header->m_active = false;
  scene->m_header_gaps.append(handler);
  handler = _TNA_RENDER_HANDLER_INVALID;
}

void
rendering_scene_set_mesh(rendering_scene_t* scene,
                         render_handler_t handler, 
                         const char* mesh)
{
  assert(handler != _TNA_RENDER_HANDLER_INVALID && "Invalid render handler");

  render_header_t* header = &scene->m_headers[handler];
  scene->m_meshes[header->m_offset] = p_mesh_registry->load(mesh);
}

void
rendering_scene_set_material(rendering_scene_t* scene,
                             render_handler_t handler, 
                             const material_desc_t& mat_desc)
{
  assert(handler != _TNA_RENDER_HANDLER_INVALID && "Invalid render handler");

  render_header_t* header = &scene->m_headers[handler];
  render_mesh_uniform_t* uniform = nullptr;
  uniform = ((render_mesh_uniform_t*)&scene->m_uniforms[header->m_offset]);
  uniform->m_material = mat_desc;
}

void
rendering_scene_get_material(rendering_scene_t* scene, 
                             render_handler_t handler, 
                                material_desc_t* mat_desc)
{
  assert(handler != _TNA_RENDER_HANDLER_INVALID && "Invalid render handler");

  render_header_t* header = &scene->m_headers[handler];
  render_mesh_uniform_t* uniform = nullptr;
  uniform = ((render_mesh_uniform_t*)&scene->m_uniforms[header->m_offset]);
  *mat_desc = uniform->m_material;
}

void
rendering_scene_set_model_mat(rendering_scene_t* scene, 
                              render_handler_t handler, 
                              const matrix4_t& mat)
{
  assert(handler != _TNA_RENDER_HANDLER_INVALID && "Invalid render handler");

  render_header_t* header = &scene->m_headers[handler];
  render_mesh_uniform_t* uniform = nullptr;
  uniform = ((render_mesh_uniform_t*)&scene->m_uniforms[header->m_offset]);
  uniform->m_model_matrix = mat;
}

void
rendering_scene_set_frustrum_culling(rendering_scene_t* scene,
                                     render_handler_t handler,
                                     const matrix4_t& proj_matrix)
{
  assert(handler != _TNA_RENDER_HANDLER_INVALID && "Invalid render handler");

  render_header_t* header = &scene->m_headers[handler];
  mesh_data_t* mesh_data = nullptr;
  render_mesh_uniform_t* uniform = nullptr;
  mesh_data = scene->m_meshes[header->m_offset];
  uniform = ((render_mesh_uniform_t*)&scene->m_uniforms[header->m_offset]);

  if(mesh_data == nullptr)
  {
    return;
  }

  const aabb_t* aabb = &mesh_data->m_aabb;
  vector4_t min = uniform->m_model_matrix*vector4_t(aabb->m_min,1.0f);
  vector4_t max = uniform->m_model_matrix*vector4_t(aabb->m_max,1.0f);
  vector4_t points[8] = {
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
    vector4_t proj_point = proj_matrix*points[i];
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
rendering_scene_set_view_matrix(rendering_scene_t* scene, 
                                const matrix4_t& view_mat) 
{
  scene->m_view_mat = view_mat;
}

void 
rendering_scene_set_proj_matrix(rendering_scene_t* scene,
                                const matrix4_t& proj_mat) 
{
  scene->m_proj_mat = proj_mat;
}

void
rendering_scene_set_clear_color(rendering_scene_t* scene,
                                const vector3_t& color)
{
  scene->m_clear_color = color;
}

} /* tna */ 
