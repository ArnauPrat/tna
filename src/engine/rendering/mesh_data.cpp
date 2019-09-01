
#include "vertex.h"
#include "furious/common/dyn_array.h"
#include <map>
#include "mesh_data.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace tna 
{

mesh_data_t*
mesh_data_create(const char* path)
{
  mesh_data_t* mesh_data = new mesh_data_t();
  mesh_data->m_aabb.m_min = {FLT_MAX, FLT_MAX, FLT_MAX};
  mesh_data->m_aabb.m_max = {FLT_MIN, FLT_MIN, FLT_MIN};

  furious::DynArray<Vertex> vertices;
  furious::DynArray<uint32_t> indices;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path)) 
  {
    TNA_LOG_ERROR("Unable to load mesh %s: %s", path, err.c_str());
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

      mesh_data->m_aabb.m_min.x = std::min(vertex.m_position.x, mesh_data->m_aabb.m_min.x);
      mesh_data->m_aabb.m_min.y = std::min(vertex.m_position.y, mesh_data->m_aabb.m_min.y);
      mesh_data->m_aabb.m_min.z = std::min(vertex.m_position.z, mesh_data->m_aabb.m_min.z);

      mesh_data->m_aabb.m_max.x = std::max(vertex.m_position.x, mesh_data->m_aabb.m_max.x);
      mesh_data->m_aabb.m_max.y = std::max(vertex.m_position.y, mesh_data->m_aabb.m_max.y);
      mesh_data->m_aabb.m_max.z = std::max(vertex.m_position.z, mesh_data->m_aabb.m_max.z);

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

      vertex.m_color = {1.0f, 1.0f, 1.0f};

      if (unique_vertices.count(index.vertex_index) == 0) 
      {
        unique_vertices[index.vertex_index] = static_cast<uint32_t>(vertices.size());
        vertices.append(vertex);
      }

      indices.append(unique_vertices[index.vertex_index]);
    }
  }


  mesh_data->m_vertex_buffer = create_vertex_buffer(vertices.buffer(),
                                                    vertices.size()*sizeof(Vertex));

  mesh_data->m_index_buffer = create_index_buffer(indices.buffer(), 
                                                  indices.size()*sizeof(uint32_t));

  mesh_data->m_num_vertices = vertices.size();
  mesh_data->m_num_indices = indices.size();

  return mesh_data;
}

void
mesh_data_destroy(mesh_data_t* mesh_data) 
{
  destroy_vertex_buffer(mesh_data->m_vertex_buffer);
  destroy_index_buffer(mesh_data->m_index_buffer);
  delete mesh_data;
  return;
}

} /* tna */ 
