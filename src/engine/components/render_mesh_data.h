

#ifndef _TNA_ENGINE_RENDER_MESH_DATA_H_
#define _TNA_ENGINE_RENDER_MESH_DATA_H_ value

#include <furious/components.h>
#include "../math/vector.h"
#include "../rendering/renderer.h"

namespace tna 
{
struct MeshData;

struct RenderMeshData
{
  FURIOUS_COMPONENT(RenderMeshData);

  const MeshData*     p_mesh_data;
  MaterialDescriptor  m_material;
};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
