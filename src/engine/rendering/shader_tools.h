

#ifndef _TNA_SHADER_TOOLS_H_
#define _TNA_SHADER_TOOLS_H_ value

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace tna {
namespace rendering {


VkShaderModule create_shader_module(VkDevice device, 
                                    const std::vector<char>& code); 

void destroy_shader_module(VkDevice device,
                           VkShaderModule shader_module);

VkPipelineShaderStageCreateInfo build_vertex_shader_stage(VkShaderModule shader_module);

VkPipelineShaderStageCreateInfo build_fragment_shader_stage(VkShaderModule shader_module);
  
}
} /* tna */ 
#endif /* ifndef _TNA_SHADERS_H_ */
