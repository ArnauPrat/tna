

#ifndef _TNA_ENGINE_MESH_H_
#define _TNA_ENGINE_MESH_H_ value

#include "../resources/resources.h"
#include <furious/components.h>
#include "../math/vector.h"

namespace tna 
{
struct MeshData;

struct Mesh 
{
  FURIOUS_COMPONENT(Mesh);

  Mesh(const std::string& name) :
  m_color(1.0f, 0.0f, 0.0f)
  {
    m_mesh_data = mesh_registry->load(name);
  }

  const MeshData* m_mesh_data;
  Vector3         m_color;
};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
