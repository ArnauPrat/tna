

#ifndef _TNA_MESHDATA_H_
#define _TNA_MESHDATA_H_

#include "../common.h"
#include "buffer.h"
#include "../shapes/aabb.h"

namespace tna 
{

struct mesh_data_t 
{
  vertex_buffer_t*  m_vertex_buffer;
  index_buffer_t*   m_index_buffer;
  uint32_t          m_num_vertices;
  uint32_t          m_num_indices;
  aabb_t            m_aabb;
};

mesh_data_t* 
mesh_data_create(const char* path);

void 
mesh_data_destroy(mesh_data_t* mesh_data);

  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
