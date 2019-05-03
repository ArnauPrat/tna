

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

  TnaRenderMeshData(const std::string& mesh)
  {
    m_handler = p_rendering_scene->create_render_object(TnaRenderObjectType::E_MESH);

    p_rendering_scene->set_mesh(m_handler, mesh);
  }

  TnaRenderHandler m_handler;
};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
