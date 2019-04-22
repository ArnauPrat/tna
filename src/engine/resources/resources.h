
#ifndef _TNA_RESOURCES_H_
#define _TNA_RESOURCES_H_ value

#include "resource_registry.h"
#include "../rendering/mesh_data.h"
#include "../rendering/shader.h"

namespace tna 
{

extern TnaResourceRegistry<TnaMeshData>* p_mesh_registry;
extern TnaResourceRegistry<TnaShader>*   p_shader_registry;

void 
init_resources();

void 
terminate_resources();
  
} /* tna */ 
#endif /* ifndef _TNA_RESOURCES_H_ */
