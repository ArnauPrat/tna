

#ifndef _TNA_MESH_H_
#define _TNA_MESH_H_ value

#include "../engine.h"

namespace tna {

struct Mesh {
  Mesh(const std::string& name) {
    m_mesh_data = resources::mesh_registry->load(name).get();
  }

  static std::string name() {
    return "Mesh";
  }

  const rendering::MeshData* m_mesh_data;
};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
