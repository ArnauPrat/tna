

#include "vkshader.h"
#include "vkshader_tools.h"
#include "../../tools/files.h"
#include "vkrenderer.h"

namespace tna {
namespace rendering {


Shader* Shader::load(const std::string& path) {

  std::vector<char> shader_code = read_file(path);

  VkShader* shader = new VkShader{};
  shader->m_shader_module = create_shader_module(m_logical_device, shader_code);
  return shader;

}

void Shader::unload(Shader* shader) {

  VkShader* vkshader = static_cast<VkShader*>(shader);

  destroy_shader_module(m_logical_device, vkshader->m_shader_module);

  delete vkshader;

}
  
}
} /* tna */ 
