


#ifndef _TNA_VKRENDERER_H_
#define _TNA_VKRENDERER_H_ value

#include <vulkan/vulkan.h>
#include "vkmem_alloc.h"

namespace tna 
{

extern VkPhysicalDevice m_physical_device;

extern VkInstance       m_vulkan_instance;

extern VkDevice         m_logical_device;

extern VkCommandPool    m_command_pool;

extern VkQueue          m_graphics_queue;

extern VkQueue          m_present_queue;

extern VmaAllocator     m_vkallocator;

} /* tna */ 

#endif /* ifndef _TNA_VKRENDERER_H_ */
