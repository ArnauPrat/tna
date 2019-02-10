
#include "resources.h"

namespace tna 
{
  
ResourceRegistry<MeshData>* mesh_registry   = nullptr;
ResourceRegistry<Shader>*   shader_registry = nullptr;

void 
init_resources() 
{
  mesh_registry = new ResourceRegistry<MeshData>();
  shader_registry = new ResourceRegistry<Shader>();
}

void 
terminate_resources() 
{
  delete shader_registry;
  delete mesh_registry;
}

} /* resources */ 
