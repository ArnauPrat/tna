

#ifndef _TNA_ENGINE_RENDER_MESH_DATA_H_
#define _TNA_ENGINE_RENDER_MESH_DATA_H_ value

#include <furious/components.h>
#include "../math/vector.h"
#include "../rendering/renderer.h"
#include "../rendering/rendering_scene.h"

namespace tna 
{
struct TnaMeshData;

struct render_mesh_data_t
{
  FURIOUS_COMPONENT(render_mesh_data_t);

  render_handler_t m_handler = _TNA_RENDER_HANDLER_INVALID;
};

void
render_mesh_data_init(render_mesh_data_t* render_mesh,
                 const char* mesh);

void
render_mesh_data_release(render_mesh_data_t* render_mesh);
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
