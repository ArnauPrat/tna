
#include "resources.h"

namespace tna 
{
  
TnaResourceRegistry<TnaMeshData>* p_mesh_registry   = nullptr;
TnaResourceRegistry<TnaShader>*   p_shader_registry = nullptr;

void 
init_resources() 
{
  p_mesh_registry = new TnaResourceRegistry<TnaMeshData>();
  p_shader_registry = new TnaResourceRegistry<TnaShader>();
}

void 
terminate_resources() 
{
  delete p_shader_registry;
  delete p_mesh_registry;
}

} /* resources */ 
