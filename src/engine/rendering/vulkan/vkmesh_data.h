
#ifndef _TNA_VKMESH_DATA_H_
#define _TNA_VKMESH_DATA_H_ value

#include "../mesh_data.h"
#include "vkmem_alloc.h"

namespace tna {
namespace rendering {

struct VkMeshData : public MeshData {

  VkMeshData() = default;
  virtual ~VkMeshData() = default;

  VkBuffer        m_vertex_buffer;
  VkBuffer        m_index_buffer;

  VmaAllocation   m_vertex_buffer_allocation;
  VmaAllocation   m_index_buffer_allocation;

  uint32_t        m_num_vertices;
  uint32_t        m_num_indices;

};

} /* rendering */ 
  
} /* tna */ 

#endif /* ifndef _TNA_VKMESH_DATA_H_ */


