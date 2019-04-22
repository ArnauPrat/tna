

#ifndef _TNA_VKSHADER_H_
#define _TNA_VKSHADER_H_ value

#include "../shader.h"
#include <vulkan/vulkan.h>

namespace tna 
{


struct VkShader 
{
  VkShaderModule m_shader_module;
};
  
} /* tna */ 
#endif /* ifndef _TNA_VKSHADER_H_ */
