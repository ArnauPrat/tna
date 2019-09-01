
#ifndef _TNA_RESOURCES_H_
#define _TNA_RESOURCES_H_ value

#include "resource_registry.h"
#include "../rendering/mesh_data.h"
#include "../rendering/shader.h"

namespace tna 
{

using mesh_registry_t = TnaResourceRegistry<mesh_data_t, mesh_data_create, mesh_data_destroy> ;
using shader_registry_t = TnaResourceRegistry<shader_t, shader_create, shader_destroy>;

extern mesh_registry_t*   p_mesh_registry;
extern shader_registry_t* p_shader_registry;

void 
init_resources();

void 
terminate_resources();
  
} /* tna */ 
#endif /* ifndef _TNA_RESOURCES_H_ */
