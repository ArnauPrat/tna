

#ifndef _TNA_VKRENDERER_TOOLS_H_
#define _TNA_VKRENDERER_TOOLS_H_ value

#include "vkrenderer.h"

namespace tna
{

VkFormat
find_supported_format(VkPhysicalDevice physical_device,
                      VkFormat* candidates, 
                      uint32_t num_candidates,
                      VkImageTiling tiling,
                      VkFormatFeatureFlags feature_flags);

VkCommandBuffer 
begin_single_time_commands(VkDevice device, 
                           VkCommandPool command_pool);

  
void 
end_single_time_commands(VkDevice device,
                         VkQueue graphics_queue,
                         VkCommandPool command_pool,
                         VkCommandBuffer command_buffer);

void
transition_image_layout(VkDevice device,
                        VkCommandPool command_pool,
                        VkQueue queue,
                        VkImage image, 
                        VkFormat format, 
                        VkImageLayout oldLayout, 
                        VkImageLayout newLayout);
} /* tna */ 



#endif /* ifndef _TNA_VKRENDERER_TOOLS_H_ */
