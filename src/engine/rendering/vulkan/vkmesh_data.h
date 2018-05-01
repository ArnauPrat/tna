
#ifndef _TNA_VKMESH_DATA_H_
#define _TNA_VKMESH_DATA_H_ value

#include "../mesh_data.h"

namespace tna {
namespace rendering {

struct VkMeshData : public MeshData {

  VkMeshData() = default;
  virtual ~VkMeshData();

  VkBuffer        m_vertex_buffer;
  VkBuffer        m_index_buffer;

  VkDeviceMemory  m_vertex_buffer_memory;
  VkDeviceMemory  m_index_buffer_memory;

};

} /* rendering */ 
  
} /* tna */ 

#endif /* ifndef _TNA_VKMESH_DATA_H_ */


