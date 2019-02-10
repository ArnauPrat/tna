

#ifndef _TNA_MESH_H_
#define _TNA_MESH_H_ value

#include "../resources/resources.h"
#include <furious/components.h>

namespace tna 
{
struct MeshData;

struct Mesh 
{
  FURIOUS_COMPONENT(Mesh);

  Mesh(const std::string& name) 
  {
    m_mesh_data = mesh_registry->load(name);
  }

  static 
  std::string name() 
  {
    return "Mesh";
  }

  const MeshData* m_mesh_data;
};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
