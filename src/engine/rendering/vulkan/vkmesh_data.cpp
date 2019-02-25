
#include "../vertex.h"
#include "vkbuffer_tools.h"
#include "vkmesh_data.h"
#include "vkrenderer.h"
#include <map>
#include <vector>

#include "vkmem_alloc.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/hash.hpp>
//
//namespace std {
//    template<> struct hash<tna::Vertex> {
//        size_t operator()(tna::Vertex const& vertex) const {
//            return ((hash<glm::vec3>()(vertex.m_position) ^
//                   (hash<glm::vec3>()(vertex.m_color) << 1)) >> 1) ^
//                   (hash<glm::vec2>()(vertex.m_tex_coord) << 1);
//        }
//    };
//}

namespace tna 
{

MeshData*
MeshData::load(const std::string& path)
{
  VkMeshData* vkmesh_data = new VkMeshData();
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) 
  {
    log->error("Unable to load mesh %s: %s", path.c_str(), err.c_str());
    report_error(TNA_ERROR::E_RENDERER_RESOURCE_ALLOCATION_ERROR);
  }

  std::map<uint32_t,uint32_t> unique_vertices;
  for (const tinyobj::shape_t& shape : shapes) 
  {
    for (const tinyobj::index_t& index : shape.mesh.indices) 
    {
      Vertex vertex;

      vertex.m_position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2]
                          };


      if(attrib.texcoords.size() != 0) 
      {
        vertex.m_tex_coord = {
          attrib.texcoords[2 * index.texcoord_index + 0],
          1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
        };
      } 
      else 
      {
        vertex.m_tex_coord = {
          0.0,
          0.0};
      }

      vertex.m_color = {1.0f, 0.0f, 0.0f};

      if (unique_vertices.count(index.vertex_index) == 0) 
      {
        unique_vertices[index.vertex_index] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(vertex);
      }

      indices.push_back(unique_vertices[index.vertex_index]);
    }
  }

  create_vertex_buffer(p_renderer->m_logical_device, 
                       p_renderer->m_graphics_queue,
                       p_renderer->m_command_pool,
                       p_renderer->m_vkallocator,
                       vertices.data(), 
                       vertices.size(),
                       &vkmesh_data->m_vertex_buffer,
                       &vkmesh_data->m_vertex_buffer_allocation);

  create_index_buffer(p_renderer->m_logical_device,
                      p_renderer->m_graphics_queue,
                      p_renderer->m_command_pool,
                      p_renderer->m_vkallocator,
                      indices.data(), 
                      indices.size(),
                      &vkmesh_data->m_index_buffer,
                      &vkmesh_data->m_index_buffer_allocation);

  vkmesh_data->m_num_vertices = vertices.size();
  vkmesh_data->m_num_indices = indices.size();

  return vkmesh_data;
}

void
MeshData::unload(MeshData* mesh_data) 
{
  VkMeshData* vkmesh_data = static_cast<VkMeshData*>(mesh_data);

  vmaDestroyBuffer(p_renderer->m_vkallocator,
                   vkmesh_data->m_vertex_buffer,
                   vkmesh_data->m_vertex_buffer_allocation);

  vmaDestroyBuffer(p_renderer->m_vkallocator,
                   vkmesh_data->m_index_buffer,
                   vkmesh_data->m_index_buffer_allocation);
  delete vkmesh_data;
  return;
}

} /* tna */ 
