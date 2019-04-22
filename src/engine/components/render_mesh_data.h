

#ifndef _TNA_ENGINE_RENDER_MESH_DATA_H_
#define _TNA_ENGINE_RENDER_MESH_DATA_H_ value

#include <furious/components.h>
#include "../math/vector.h"
#include "../rendering/renderer.h"
#include "../rendering/rendering_scene.h"

namespace tna 
{
struct TnaMeshData;

struct TnaRenderMeshData
{
  FURIOUS_COMPONENT(TnaRenderMeshData);

  const TnaMeshData*     p_mesh_data;
  TnaMaterialDescriptor  m_material;
};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
