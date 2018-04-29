

#include "../tools/files.h"
#include "../common.h"

namespace tna {
namespace rendering {


VkShaderModule create_shader_module(VkDevice device, 
                                    const std::vector<char>& code) {

  VkShaderModuleCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shader_module;
  if (vkCreateShaderModule(device, 
                           &create_info, 
                           nullptr, 
                           &shader_module) != VK_SUCCESS) {

    throw std::runtime_error("failed to create shader module!");
  }

  return shader_module;

}

void destroy_shader_module(VkDevice device, VkShaderModule shader_module) {

  vkDestroyShaderModule(device, shader_module, nullptr);

}

VkPipelineShaderStageCreateInfo build_vertex_shader_stage(VkShaderModule shader_module) {

  VkPipelineShaderStageCreateInfo vertex_shader_stage_info = {};
  vertex_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertex_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertex_shader_stage_info.module = shader_module;
  vertex_shader_stage_info.pName = "main";
  return vertex_shader_stage_info;
}

VkPipelineShaderStageCreateInfo build_fragment_shader_stage(VkShaderModule shader_module) {
	VkPipelineShaderStageCreateInfo fragment_shader_stage_info = {};
	fragment_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragment_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragment_shader_stage_info.module = shader_module;
	fragment_shader_stage_info.pName = "main";
  return fragment_shader_stage_info;
}

}
} /* tna */ 
