
#include "../vertex.h"
#include "vkbuffer_tools.h"
#include "vkmesh_data.h"
#include "vkrenderer.h"
#include <unordered_map>
#include <vector>

#include "vkmem_alloc.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {
    template<> struct hash<tna::rendering::Vertex> {
        size_t operator()(tna::rendering::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.m_position) ^
                   (hash<glm::vec3>()(vertex.m_color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.m_tex_coord) << 1);
        }
    };
}

namespace tna {
namespace rendering {


MeshData* MeshData::load(const std::string& path ) {


  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) {
    throw std::runtime_error(err);
  }

  std::unordered_map<Vertex, uint32_t> unique_vertices = {};
  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      Vertex vertex = {};

      vertex.m_position = {
        attrib.vertices[3 * index.vertex_index + 0],
        attrib.vertices[3 * index.vertex_index + 1],
        attrib.vertices[3 * index.vertex_index + 2]
      };


      vertex.m_tex_coord = {
        attrib.texcoords[2 * index.texcoord_index + 0],
        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
      };

      vertex.m_color = {1.0f, 1.0f, 1.0f};


      if (unique_vertices.count(vertex) == 0) {
        unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(vertex);
      }

      indices.push_back(indices.size());
    }
  }

  VkMeshData* mesh_data = new VkMeshData{};

  create_vertex_buffer(vertices, 
                       mesh_data->m_vertex_buffer,
                       mesh_data->m_vertex_buffer_allocation);

  create_index_buffer(indices, 
                      mesh_data->m_index_buffer,
                      mesh_data->m_index_buffer_allocation);


  return mesh_data;
}

void MeshData::unload(MeshData* mesh_data) {
  VkMeshData* vkmesh_data = static_cast<VkMeshData*>(mesh_data);

  vmaDestroyBuffer(m_vkallocator,
                   vkmesh_data->m_vertex_buffer,
                   vkmesh_data->m_vertex_buffer_allocation);

  vmaDestroyBuffer(m_vkallocator,
                   vkmesh_data->m_index_buffer,
                   vkmesh_data->m_index_buffer_allocation);
  delete vkmesh_data;
}
  
}
} /* tna */ 
