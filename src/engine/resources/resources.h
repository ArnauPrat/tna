
#ifndef _TNA_RESOURCES_H_
#define _TNA_RESOURCES_H_ value

#include "resource_registry.h"
#include "../rendering/mesh_data.h"
#include "../rendering/shader.h"

namespace tna {
namespace resources {

extern ResourceRegistry<rendering::MeshData>* mesh_registry;
extern ResourceRegistry<rendering::Shader>*   shader_registry;

void init_resources();

void terminate_resources();
  
} /* resources */ 
} /* tna */ 
#endif /* ifndef _TNA_RESOURCES_H_ */
