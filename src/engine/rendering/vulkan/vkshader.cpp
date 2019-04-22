

#include "vkshader.h"
#include "vkshader_tools.h"
#include "../../tools/files.h"
#include "vkrenderer.h"

namespace tna {


TnaShader* 
TnaShader::load(const std::string& path) 
{

  std::vector<char> shader_code = read_file(path);

  VkShader* vkshader = new VkShader();
  vkshader->m_shader_module = create_shader_module(p_renderer->m_logical_device, 
                                                   shader_code);
  return new TnaShader{vkshader};
}

void 
TnaShader::unload(TnaShader* shader) 
{

  VkShader* vkshader = static_cast<VkShader*>(shader->p_data);

  destroy_shader_module(p_renderer->m_logical_device, 
                        vkshader->m_shader_module);

  delete vkshader;
  delete shader;
}
  
} /* tna */ 
