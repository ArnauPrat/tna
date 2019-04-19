

#ifndef _TNA_MESHDATA_H_
#define _TNA_MESHDATA_H_

#include "../common.h"
#include "buffer.h"
#include <string>
#include "../shapes/aabb.h"

namespace tna 
{

struct AABB;
  
struct MeshData 
{
  VertexBuffer  m_vertex_buffer;
  IndexBuffer   m_index_buffer;
  uint32_t      m_num_vertices;
  uint32_t      m_num_indices;
  AABB          m_aabb;

  static MeshData* 
  load(const std::string& path);

  static void 
  unload(MeshData* mesh_data);
};

  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
