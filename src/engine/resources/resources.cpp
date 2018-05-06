
#include "resources.h"

namespace tna {
namespace resources {
  
ResourceRegistry<rendering::MeshData>* mesh_registry   = nullptr;
ResourceRegistry<rendering::Shader>*   shader_registry = nullptr;

void init_resources() {
  mesh_registry = new ResourceRegistry<rendering::MeshData>();
  shader_registry = new ResourceRegistry<rendering::Shader>();
}

void terminate_resources() {
  delete shader_registry;
  delete mesh_registry;
}

} /* tna */ 
} /* resources */ 
