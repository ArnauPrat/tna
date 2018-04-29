

#ifndef _TNA_MESH_H_
#define _TNA_MESH_H_ value

#include "vertex.h"
#include "../types.h"

namespace tna {

struct Mesh {
  const Vertex*   p_vdata;
  const uint32_t m_num_vertices;
};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
