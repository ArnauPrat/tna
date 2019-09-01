



#include "../../common.h"
#include "vkrenderer_tools.h"

namespace tna
{

VkFormat
find_supported_format(VkPhysicalDevice physical_device, 
                      VkFormat* candidates, 
                      uint32_t num_candidates,
                      VkImageTiling tiling,
                      VkFormatFeatureFlags feature_flags)
{
  for (uint32_t i = 0; i < num_candidates; ++i) 
  {
    VkFormat format = candidates[i];
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physical_device, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & feature_flags) == feature_flags) 
    {
      return format;
    } 
    else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & feature_flags) == feature_flags) 
    {
      return format;
    }
  }
  return VK_FORMAT_UNDEFINED;
}



VkCommandBuffer 
begin_single_time_commands(VkDevice device, 
                           VkCommandPool command_pool) 
{
  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = command_pool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(device, &allocInfo, &command_buffer);

  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(command_buffer, &beginInfo);

  return command_buffer;
}

void 
end_single_time_commands(VkDevice device,
                         VkQueue graphics_queue,
                         VkCommandPool command_pool,
                         VkCommandBuffer command_buffer) 
{
    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command_buffer;

    vkQueueSubmit(graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphics_queue);

    vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
}

void 
transition_image_layout(VkDevice device,
                        VkCommandPool command_pool,
                        VkQueue queue,
                        VkImage image, 
                        VkFormat format, 
                        VkImageLayout oldLayout, 
                        VkImageLayout newLayout) 
{
    VkCommandBuffer command_buffer = begin_single_time_commands(device, command_pool);

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

      if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) // has stencil buffer
      {
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
      }
    } else {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }


    VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED 
        && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
    {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } 
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL 
             && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
    {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } 
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED 
             && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {

      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else 
    {
      TNA_LOG_ERROR("unsupported layout transition!");
      report_error(TNA_ERROR::E_RENDERER_RESOURCE_ALLOCATION_ERROR);
    }


    vkCmdPipelineBarrier(command_buffer,
                         sourceStage, 
                         destinationStage,
                         0,
                         0, 
                         nullptr,
                         0, 
                         nullptr,
                         1, 
                         &barrier);

    end_single_time_commands(device, 
                             queue, 
                             command_pool, 
                             command_buffer);
}

} /* tna */ 
