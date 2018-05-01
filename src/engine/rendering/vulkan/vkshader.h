

#ifndef _TNA_VKSHADER_H_
#define _TNA_VKSHADER_H_ value

#include "../shader.h"

namespace tna {
namespace rendering {

struct VkShader : Shader {

  VkShaderModule  m_shader_module;

};
  
} /* rendering */ 
} /* tna */ 
#endif /* ifndef _TNA_VKSHADER_H_ */
