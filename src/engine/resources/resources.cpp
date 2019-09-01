
#include "resources.h"

namespace tna 
{
  

mesh_registry_t* p_mesh_registry = nullptr;;
shader_registry_t* p_shader_registry = nullptr;;

void 
init_resources() 
{
  p_mesh_registry = new mesh_registry_t();
  p_shader_registry = new shader_registry_t();
}

void 
terminate_resources() 
{
  delete p_shader_registry;
  delete p_mesh_registry;
}

} /* resources */ 
