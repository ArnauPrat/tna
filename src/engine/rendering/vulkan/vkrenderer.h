


#ifndef _TNA_VKRENDERER_H_
#define _TNA_VKRENDERER_H_ value

#include <vulkan/vulkan.h>
#include "vkmem_alloc.h"
#include "vkdevice.h"

#include <vector>

namespace tna 
{

struct VulkanRenderer 
{

  VulkanRenderer();
  ~VulkanRenderer();

  uint32_t                      m_viewport_width;   ///< Width of the viewport
  uint32_t                      m_viewport_height;  ///< Height of the viewport

  VkInstance                    m_vulkan_instance;  ///< Vulkan instance
  VkSurfaceKHR                  m_window_surface;   ///< Rendering surface
  VkDebugReportCallbackEXT      m_report_callback;  ///< Debug report callback
  VkPhysicalDevice              m_physical_device;  ///< Physical device
  VkDevice                      m_logical_device ;  ///< Logical device
  VkSwapchainKHR                m_swap_chain;       ///< Swap chain
  VkPipelineLayout              m_pipeline_layout;  ///< pipeline layout
  VkRenderPass                  m_render_pass;      ///< Render pass object
  VkPipeline                    m_pipeline;         ///< Rendering pipeline
  VkCommandPool                 m_command_pool;     ///< Rendering command pool
  VkDescriptorPool              m_descriptor_pool;  ///< The descriptor pool
  VkDescriptorSetLayout         m_descriptor_set_layout; ///< The descriptor set layout
  VkQueue                       m_graphics_queue;   ///< Graphics command queue
  VkQueue                       m_present_queue;    ///< Present command queue

  VkPhysicalDeviceProperties    m_mem_properties;   ///< Memory properties
  VkSurfaceFormatKHR            m_surface_format;   ///< Rendering Surface format
  VkExtent2D                    m_extent;           ///< Rendering Surface extent
  QueueFamilyIndices 			      m_queue_indices;    ///< Queue family indices

  VkFormat                      m_depth_format;
  VkImage                       m_depth_image;            ///< The depth buffer image
  VkImageView                   m_depth_image_view;       ///< The depth buffer image view
  VmaAllocation                 m_depth_image_allocation; ///< The depth buffer image allocation

  VkImage*                      m_swap_chain_images;       ///< Vector with the different swap chain images
  VkImageView*                  m_swap_chain_image_views;  ///< Vector with the different swap chain image views
  VkFramebuffer*                m_frame_buffers;           ///< Vector with the different frame buffers. One per swap chain image
  VkCommandBuffer*              m_command_buffers;    ///< Vector with the different command buffers. One per swap chain image 
  VkDescriptorSet*              m_descriptor_sets;   ///< The descriptor set of the pipeline
  const char**                  m_validation_layers;  ///< validation layers
  const char**                  m_device_extensions;  ///< Vector with the required physical device extensions

  uint32_t                      m_num_swap_chain_images;      ///< Number of swap chain images
  uint32_t                      m_num_swap_chain_image_views; ///< Number of wap chain image views
  uint32_t                      m_num_validation_layers;      ///< Number of validation layers
  uint32_t                      m_num_device_extensions;      ///< Number of required physical device extensions
  uint32_t                      m_num_frame_buffers;          ///< Number of frame buffers 
  uint32_t                      m_num_command_buffers;        ///< Number of command buffers
  uint32_t                      m_num_descriptor_sets;        ///< Number of command buffers


  VkSemaphore                   m_image_available_semaphore; ///< Semaphore for synchronizing next frame image retrieval
  VkSemaphore                   m_render_finished_semaphore; ///< Sempahore to synchronize render pass finished

  VmaAllocator                  m_vkallocator;    ///< The memory allocator used to create Vulkan buffers
};

extern VulkanRenderer*     p_renderer;
//
//extern VkInstance       m_vulkan_instance;
//
//extern VkDevice         m_logical_device;
//
//extern VkCommandPool    m_command_pool;
//
//extern VkQueue          m_graphics_queue;
//
//extern VkQueue          m_present_queue;
//
//extern VmaAllocator     m_vkallocator;

} /* tna */ 

#endif /* ifndef _TNA_VKRENDERER_H_ */
