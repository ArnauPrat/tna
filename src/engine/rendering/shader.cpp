

#include "shader.h"
#include "shader_tools.h"
#include "../tools/files.h"
#include "renderer.h"

namespace tna {
namespace rendering {


Shader* Shader::load(const std::string& path) {

  std::vector<char> vertex_shader_code = read_file("vert.spv");

  Shader* shader = new Shader{};
  shader->m_shader_module = create_shader_module(rendering::logical_device(), vertex_shader_code);
  return shader;

}

void Shader::unload(Shader* shader) {

  destroy_shader_module(rendering::logical_device(), shader->m_shader_module);

}
  
}
} /* tna */ 
