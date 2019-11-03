

#include "../../common.h"
#include <vector>

namespace tna 
{

VkShaderModule
create_shader_module(VkDevice device, 
                     const std::vector<char>& code) 
{
  VkShaderModule shader_module;
  VkShaderModuleCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(device, 
                           &create_info, 
                           nullptr, 
                           &shader_module) != VK_SUCCESS) 
  {
    TNA_LOG_ERROR(TNA_ERROR::E_RENDERER_RESOURCE_ALLOCATION_ERROR, 
                  "failed to create shader module!");
  }
  return shader_module;
}

void 
destroy_shader_module(VkDevice device, VkShaderModule shader_module) 
{
  vkDestroyShaderModule(device, shader_module, nullptr);
  return;
}

VkPipelineShaderStageCreateInfo
build_vertex_shader_stage(VkShaderModule shader_module) 
{
  VkPipelineShaderStageCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  info.module = shader_module;
  info.pName = "main";
  info.flags = 0;
  info.pNext = nullptr;
  info.pSpecializationInfo = nullptr;
  return info;
}

VkPipelineShaderStageCreateInfo 
build_fragment_shader_stage(VkShaderModule shader_module) 
{
  VkPipelineShaderStageCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  info.module = shader_module;
  info.pName = "main";
  info.flags = 0;
  info.pNext = nullptr;
  info.pSpecializationInfo = nullptr;
  return info;
}

} /* tna */ 
