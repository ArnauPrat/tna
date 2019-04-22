

#ifndef _TNA_MESHDATA_H_
#define _TNA_MESHDATA_H_

#include "../common.h"
#include "buffer.h"
#include <string>
#include "../shapes/aabb.h"

namespace tna 
{

struct TnaMeshData 
{
  TnaVertexBuffer  m_vertex_buffer;
  TnaIndexBuffer   m_index_buffer;
  uint32_t         m_num_vertices;
  uint32_t         m_num_indices;
  TnaAABB          m_aabb;

  static TnaMeshData* 
  load(const std::string& path);

  static void 
  unload(TnaMeshData* mesh_data);
};

  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
