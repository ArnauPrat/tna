


#include "../../common.h"
#include "../../config.h"
#include "../../resources/resources.h"
#include "../../tools/files.h"
#include "../vertex.h"
#include "../../math/matrix.h"
#include "../../math/math_tools.h"


#define VMA_IMPLEMENTATION
#include "vkmem_alloc.h"

#include "vkbuffer_tools.h"
#include "vkmesh_data.h"
#include "vkscene.h"
#include "vkshader.h"
#include "vkshader_tools.h"
#include "vkvertex_tools.h"
#include "vkdevice.h"
#include "vkswapchain.h"

#include "string.h"

namespace tna 
{

#define MAX_PRIMITIVE_COUNT 65536

/**
 * \brief Flag used to enable validation layers or not
 */
#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif


VulkanRenderer*                 p_renderer; ///<  This is the VulkanRenderer

VkScene                         m_scene; ///<  The Vulkan Scene object to store the scene to render information
Matrix4                         m_projection_matrix; ///< The projection matrix
Matrix4                         m_view_matrix; ///< The view matrix

struct UniformBufferObject 
{
    Matrix4 projmodelview;
};


uint32_t               m_num_uniform_buffers;
VkBuffer*              m_uniform_buffers;
VmaAllocation*         m_uniform_allocations;

void*                 m_udata = nullptr;

void 
clean_up_graphics_pipeline();
void
clean_up_image_views();
void
clean_up_swap_chain();

VulkanRenderer::VulkanRenderer() : 
  m_viewport_width(0),                
  m_viewport_height(0),                
  m_vulkan_instance(VK_NULL_HANDLE),   
  m_window_surface(VK_NULL_HANDLE),   
  m_report_callback(VK_NULL_HANDLE),   
  m_physical_device(VK_NULL_HANDLE),
  m_logical_device(VK_NULL_HANDLE),
  m_swap_chain(VK_NULL_HANDLE),   
  m_pipeline_layout(VK_NULL_HANDLE),  
  m_render_pass(VK_NULL_HANDLE),  
  m_pipeline(VK_NULL_HANDLE),  
  m_command_pool(VK_NULL_HANDLE),  
  m_descriptor_pool(VK_NULL_HANDLE), 
  m_descriptor_set_layout (VK_NULL_HANDLE), 
  m_graphics_queue(VK_NULL_HANDLE), 
  m_present_queue(VK_NULL_HANDLE),
  m_swap_chain_images(nullptr),
  m_swap_chain_image_views(nullptr), 
  m_frame_buffers(nullptr),
  m_command_buffers(nullptr),
  m_descriptor_sets(nullptr),
  m_validation_layers(nullptr),  
  m_device_extensions(nullptr), 
  m_num_swap_chain_images(0),  
  m_num_swap_chain_image_views(0), 
  m_num_validation_layers(0),    
  m_num_device_extensions(0),   
  m_num_frame_buffers(0),   
  m_num_command_buffers(0),
  m_num_descriptor_sets(0),
  m_image_available_semaphore(VK_NULL_HANDLE),
  m_render_finished_semaphore(VK_NULL_HANDLE)
{
}

VulkanRenderer::~VulkanRenderer()
{

  if(p_renderer->m_render_finished_semaphore != VK_NULL_HANDLE)
  {
    vkDestroySemaphore(p_renderer->m_logical_device, 
                       p_renderer->m_render_finished_semaphore, 
                       nullptr);
    p_renderer->m_render_finished_semaphore = VK_NULL_HANDLE;
  }

  if(p_renderer->m_image_available_semaphore != VK_NULL_HANDLE)
  {
    vkDestroySemaphore(p_renderer->m_logical_device, 
                       p_renderer->m_image_available_semaphore, 
                       nullptr);
    p_renderer->m_image_available_semaphore = VK_NULL_HANDLE;
  }

  clean_up_graphics_pipeline();
  clean_up_image_views();
  clean_up_swap_chain();

  if(p_renderer->m_descriptor_sets)
  {
    delete [] p_renderer->m_descriptor_sets;
    p_renderer->m_descriptor_sets = nullptr;
    p_renderer->m_num_descriptor_sets = 0;
  }

  if(p_renderer->m_descriptor_pool != VK_NULL_HANDLE)
  {
    vkDestroyDescriptorPool(p_renderer->m_logical_device, 
                            p_renderer->m_descriptor_pool, 
                            nullptr);
    p_renderer->m_descriptor_pool = VK_NULL_HANDLE;
  }

  if(p_renderer->m_descriptor_set_layout != VK_NULL_HANDLE)
  {
    vkDestroyDescriptorSetLayout(p_renderer->m_logical_device, 
                                 p_renderer->m_descriptor_set_layout, 
                                 nullptr);
    p_renderer->m_descriptor_set_layout = VK_NULL_HANDLE;
  }

  if(p_renderer->m_command_pool != VK_NULL_HANDLE)
  {
    vkDestroyCommandPool(p_renderer->m_logical_device, 
                         p_renderer->m_command_pool, 
                         nullptr);
    p_renderer->m_command_pool = VK_NULL_HANDLE;
  }

  if(m_command_buffers)
  {
    delete [] m_command_buffers;
    m_command_buffers = nullptr;;
    m_num_command_buffers = 0;
  }

  vmaDestroyAllocator(p_renderer->m_vkallocator);

  if(p_renderer->m_logical_device != VK_NULL_HANDLE)
  {
    vkDestroyDevice(p_renderer->m_logical_device, 
                    nullptr);
    p_renderer->m_logical_device = VK_NULL_HANDLE;
  }

  if(p_renderer->m_window_surface != VK_NULL_HANDLE)
  {
    vkDestroySurfaceKHR(p_renderer->m_vulkan_instance, 
                        p_renderer->m_window_surface, 
                        nullptr);
    p_renderer->m_window_surface = VK_NULL_HANDLE;
  }

  if (enable_validation_layers) 
  {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(p_renderer->m_vulkan_instance, "vkDestroyDebugReportCallbackEXT");
    func(p_renderer->m_vulkan_instance, p_renderer->m_report_callback, nullptr);
  }

  if(p_renderer->m_vulkan_instance != VK_NULL_HANDLE)
  {
    vkDestroyInstance(p_renderer->m_vulkan_instance, nullptr);
    p_renderer->m_vulkan_instance = VK_NULL_HANDLE;
  }


  if(m_validation_layers)
  {
    for(uint32_t i = 0; i < m_num_validation_layers; ++i)
    {
      delete [] m_validation_layers[i];
    }
    delete [] m_validation_layers;
    m_validation_layers = nullptr;
    m_num_validation_layers = 0; 
  }

  if(m_device_extensions)
  {
    for(uint32_t i = 0; i < m_num_device_extensions; ++i)
    {
      delete [] m_device_extensions[i];
    }
    delete [] m_device_extensions;
    m_device_extensions = nullptr;
    m_num_device_extensions = 0;
  }
}

/**
 * \brief Callback used to capture debug information
 *
 * \param flags
 * \param objType
 * \param obj
 * \param location
 * \param code
 * \param layerPrefix
 * \param msg
 * \param userData
 *
 * \return 
 */
VKAPI_ATTR VkBool32 VKAPI_CALL 
debug_callback(VkDebugReportFlagsEXT flags,
               VkDebugReportObjectTypeEXT objType,
               uint64_t obj,
               size_t location,
               int32_t code,
               const char* layerPrefix,
               const char* msg,
               void* userData) 
{
  log->error("validation layer: %s", msg);
  report_error(TNA_ERROR::E_RENDERER_VULKAN_ERROR);
  return VK_FALSE;
}


void 
create_vulkan_instance() 
{
  // Getting Available Validation Layers
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  VkLayerProperties* available_layers = new VkLayerProperties[layer_count];
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers); 

  for (uint32_t i = 0; i < p_renderer->m_num_validation_layers; ++i) 
  {
    const char* layer = p_renderer->m_validation_layers[i];
    bool layer_found = false;
    for (uint32_t j = 0; j < layer_count; ++j) 
    {
      VkLayerProperties* next_layer = &available_layers[j];
      if (strcmp(layer, next_layer->layerName) == 0) 
      {
        log->log("Found validation layer %s", layer);
        layer_found = true;
        break;
      }
    }

    if (!layer_found) 
    {
      log->error("Validation layer %s not found", layer);
      report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
    }
  }
  delete [] available_layers;

  // Creating Vulkan Application Information
  VkApplicationInfo app_info = {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "TNA";
  app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  app_info.pEngineName = "TNA Engine";
  app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  app_info.apiVersion = VK_API_VERSION_1_0;

  // Creating Vulkan Instance Information
  VkInstanceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  if(enable_validation_layers) 
  {
    create_info.enabledLayerCount = p_renderer->m_num_validation_layers;
    create_info.ppEnabledLayerNames = p_renderer->m_validation_layers;
  }

  // Adding GLFW and other required extensions
  unsigned int glfw_extensions_count = 0;
  const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extensions_count);
  
  uint32_t extensions_count = glfw_extensions_count;
  if(enable_validation_layers) 
  {
    extensions_count += 1;
  }

  const char** extensions = new const char*[extensions_count];
  for(uint32_t i = 0; i < glfw_extensions_count; ++i) 
  {
    extensions[i] = glfw_extensions[i];
  }

  if(enable_validation_layers) 
  {
    extensions[extensions_count - 1] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
  }

  create_info.enabledExtensionCount = extensions_count;
  create_info.ppEnabledExtensionNames = extensions;

  if(vkCreateInstance(&create_info, 
                      nullptr, 
                      &p_renderer->m_vulkan_instance) != VK_SUCCESS) 
  {
    log->error("failed to create instance!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }
  delete [] extensions;
  return;
}

void
create_surface(GLFWwindow* window) 
{
  if (glfwCreateWindowSurface(p_renderer->m_vulkan_instance, 
                              window, 
                              nullptr, 
                              &p_renderer->m_window_surface) != VK_SUCCESS) 
  {
    log->error("failed to create window surface!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }
  return;
}

void
add_debug_handler() 
{
  // Adding debug callback
  if(enable_validation_layers) {
    VkDebugReportCallbackCreateInfoEXT debug_callback_info = {};
    debug_callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    debug_callback_info.pfnCallback = debug_callback;

    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(p_renderer->m_vulkan_instance, "vkCreateDebugReportCallbackEXT");
    if(func(p_renderer->m_vulkan_instance, 
            &debug_callback_info, 
            nullptr, 
            &p_renderer->m_report_callback) != VK_SUCCESS) 
    {
      log->error("Failed to create debug callback!");
      report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
    }
  }
  return;
}

void
create_physical_device() 
{
  // Adding the required device extensions
  p_renderer->m_num_device_extensions = 1;
  p_renderer->m_device_extensions = new const char*[p_renderer->m_num_device_extensions];
  p_renderer->m_device_extensions[0] = new char[strlen(VK_KHR_SWAPCHAIN_EXTENSION_NAME)+1];
  strcpy((char*)p_renderer->m_device_extensions[0], VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	// Creating Physical Device
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(p_renderer->m_vulkan_instance, 
                             &device_count, 
                             nullptr);

  VkPhysicalDevice* physical_devices = new VkPhysicalDevice[device_count];

  vkEnumeratePhysicalDevices(p_renderer->m_vulkan_instance, 
                             &device_count, 
                             physical_devices);


  for (uint32_t i = 0; 
       i < device_count && p_renderer->m_physical_device == VK_NULL_HANDLE; 
       ++i) 
  {
    QueueFamilyIndices indices;

		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], 
                                             &queue_family_count, 
                                             nullptr);

		VkQueueFamilyProperties * queue_families = new VkQueueFamilyProperties[queue_family_count];
		vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], 
                                             &queue_family_count, 
                                             queue_families);

    for(uint32_t j = 0; j < queue_family_count; ++j)
    {
      VkQueueFamilyProperties* queue_family = &queue_families[j];
			if (queue_family->queueCount > 0 && queue_family->queueFlags & VK_QUEUE_GRAPHICS_BIT) 
      {
				indices.m_graphics_queue = j;
			}

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(physical_devices[i], 
                                           j, 
                                           p_renderer->m_window_surface, 
                                           &presentSupport);

      if(presentSupport) 
      {
        indices.m_present_queue = j;
      }
		}

    if (is_device_suitable(physical_devices[i], 
                           p_renderer->m_window_surface,
                           &indices,
                           p_renderer->m_device_extensions,
                           p_renderer->m_num_device_extensions)) 
    {
      p_renderer->m_physical_device = physical_devices[i];
      p_renderer->m_queue_indices = indices;
    }
    delete [] queue_families;
  }
  delete [] physical_devices;

  if (p_renderer->m_physical_device == VK_NULL_HANDLE) 
  {
    log->error("failed to find a suitable GPU!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  vkGetPhysicalDeviceProperties(p_renderer->m_physical_device, &p_renderer->m_mem_properties);
  return;
}

void
create_logical_device() 
{

  uint32_t num_distinct_queue_families = 1;
  if(p_renderer->m_queue_indices.m_graphics_queue != 
     p_renderer->m_queue_indices.m_present_queue)
  {
    ++num_distinct_queue_families;
  }

  // Creating Queue creation information 
  VkDeviceQueueCreateInfo* queue_create_infos = new VkDeviceQueueCreateInfo[num_distinct_queue_families];

  float queue_priority = 1.0f;
  if(num_distinct_queue_families == 1)
  {
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = p_renderer->m_queue_indices.m_graphics_queue;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos[0] = queue_create_info;
  }
  else
  {
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = p_renderer->m_queue_indices.m_graphics_queue;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos[0] = queue_create_info;

    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = p_renderer->m_queue_indices.m_present_queue;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos[1] = queue_create_info;
  }

  VkPhysicalDeviceFeatures device_features = {};

  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = queue_create_infos;
  createInfo.queueCreateInfoCount = num_distinct_queue_families;
  createInfo.pEnabledFeatures = &device_features;
  createInfo.enabledExtensionCount = p_renderer->m_num_device_extensions;
  createInfo.ppEnabledExtensionNames = p_renderer->m_device_extensions;

  if (enable_validation_layers) 
  {
    createInfo.enabledLayerCount = p_renderer->m_num_validation_layers;
    createInfo.ppEnabledLayerNames = p_renderer->m_validation_layers;
  } 
  else 
  {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(p_renderer->m_physical_device, 
                     &createInfo, 
                     nullptr, 
                     &p_renderer->m_logical_device) != VK_SUCCESS) 
  {
    log->error("failed to create logical device!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  delete [] queue_create_infos;
  return;
}

void
create_command_queues() 
{
  // Retrieving queues
  vkGetDeviceQueue(p_renderer->m_logical_device, 
                   p_renderer->m_queue_indices.m_graphics_queue, 
                   0, 
                   &p_renderer->m_graphics_queue);

  vkGetDeviceQueue(p_renderer->m_logical_device, 
                   p_renderer->m_queue_indices.m_present_queue, 
                   0, 
                   &p_renderer->m_present_queue);
  return;
}

void
create_swap_chain() 
{
  // Creating Swap chain
  SwapChainSupportDetails swap_chain_support; 
  query_sc_support_details(p_renderer->m_physical_device, 
                           p_renderer->m_window_surface,
                           &swap_chain_support);

  p_renderer->m_surface_format = choose_sc_surface_format(swap_chain_support.m_formats,
                                                         swap_chain_support.m_num_formats);

  VkPresentModeKHR present_mode = choose_sc_present_mode(swap_chain_support.m_present_modes,
                                                         swap_chain_support.m_num_present_modes);

  p_renderer->m_extent = choose_sc_extent(swap_chain_support.m_capabilities, 
                                         p_renderer->m_viewport_width, 
                                         p_renderer->m_viewport_height);

  uint32_t image_count = swap_chain_support.m_capabilities.minImageCount + 1;

  if (swap_chain_support.m_capabilities.maxImageCount > 0 && 
      image_count > swap_chain_support.m_capabilities.maxImageCount) 
  {
    image_count = swap_chain_support.m_capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR swap_chain_create_info = {};
  swap_chain_create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swap_chain_create_info.surface          = p_renderer->m_window_surface;
  swap_chain_create_info.minImageCount    = image_count;
  swap_chain_create_info.imageFormat      = p_renderer->m_surface_format.format;
  swap_chain_create_info.imageColorSpace  = p_renderer->m_surface_format.colorSpace;
  swap_chain_create_info.imageExtent      = p_renderer->m_extent;
  swap_chain_create_info.imageArrayLayers = 1;
  swap_chain_create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  uint32_t queue_family_indices[] = {(uint32_t)p_renderer->m_queue_indices.m_graphics_queue, 
                                    (uint32_t) p_renderer->m_queue_indices.m_present_queue};

  if (p_renderer->m_queue_indices.m_graphics_queue != p_renderer->m_queue_indices.m_present_queue) 
  {
    swap_chain_create_info.imageSharingMode       = VK_SHARING_MODE_CONCURRENT;
    swap_chain_create_info.queueFamilyIndexCount  = 2;
    swap_chain_create_info.pQueueFamilyIndices    = queue_family_indices;
  } 
  else 
  {
    swap_chain_create_info.imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE;
    swap_chain_create_info.queueFamilyIndexCount  = 0; // Optional
    swap_chain_create_info.pQueueFamilyIndices    = nullptr; // Optional
  }

  swap_chain_create_info.preTransform   = swap_chain_support.m_capabilities.currentTransform;
  swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swap_chain_create_info.presentMode    = present_mode;
  swap_chain_create_info.clipped        = VK_TRUE;

  swap_chain_create_info.oldSwapchain   = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(p_renderer->m_logical_device, 
                           &swap_chain_create_info, 
                           nullptr, 
                           &p_renderer->m_swap_chain) != VK_SUCCESS) 
  {
    log->error("failed to create swap chain!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  vkGetSwapchainImagesKHR(p_renderer->m_logical_device, 
                          p_renderer->m_swap_chain, 
                          &p_renderer->m_num_swap_chain_images, 
                          nullptr);

  p_renderer->m_swap_chain_images = new VkImage[p_renderer->m_num_swap_chain_images];
  vkGetSwapchainImagesKHR(p_renderer->m_logical_device, 
                          p_renderer->m_swap_chain, 
                          &p_renderer->m_num_swap_chain_images, 
                          p_renderer->m_swap_chain_images);
  return;
}

void
create_image_views() 
{
  p_renderer->m_num_swap_chain_image_views = p_renderer->m_num_swap_chain_images;
  p_renderer->m_swap_chain_image_views = new VkImageView[p_renderer->m_num_swap_chain_image_views];

  for(size_t i = 0; i < p_renderer->m_num_swap_chain_image_views; ++i) 
  {
    VkImageViewCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = p_renderer->m_swap_chain_images[i];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = p_renderer->m_surface_format.format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(p_renderer->m_logical_device, 
                          &create_info, 
                          nullptr, 
                          &p_renderer->m_swap_chain_image_views[i]) != VK_SUCCESS) 
    {
      log->error("failed to create image views!");
      report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
    }
  }
  return;
}

void
create_graphics_pipeline() 
{
  Shader* vertex_shader = shader_registry->load("shaders/vert.spv");
  Shader* fragment_shader = shader_registry->load("shaders/frag.spv");
  
  if(!vertex_shader) 
  {
    log->error("Vertex shader not found");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  if(!fragment_shader) 
  {
    log->error("Fragment shader not found");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  VkShader* vkvertex_shader = static_cast<VkShader*>(vertex_shader);
  VkShader* vkfragment_shader = static_cast<VkShader*>(fragment_shader);

  VkPipelineShaderStageCreateInfo vertex_shader_stage = build_vertex_shader_stage(vkvertex_shader->m_shader_module);

  VkPipelineShaderStageCreateInfo fragment_shader_stage = build_fragment_shader_stage(vkfragment_shader->m_shader_module);

  VkPipelineShaderStageCreateInfo shader_stages[] = {vertex_shader_stage, fragment_shader_stage};

  VkVertexInputBindingDescription binding_description = get_binding_description();
  VkVertexInputAttributeDescription input_attr_desc[3];
  get_attribute_descriptions(input_attr_desc);

  VkPipelineVertexInputStateCreateInfo vertex_infput_info = {};
  vertex_infput_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_infput_info.vertexBindingDescriptionCount = 1;
  vertex_infput_info.pVertexBindingDescriptions = &binding_description; // Optional
  vertex_infput_info.vertexAttributeDescriptionCount = 3;
  vertex_infput_info.pVertexAttributeDescriptions = input_attr_desc; // Optional

  VkPipelineInputAssemblyStateCreateInfo input_assembly = {};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float) p_renderer->m_extent.width;
  viewport.height = (float) p_renderer->m_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor = {};
  scissor.offset = {0, 0};
  scissor.extent = p_renderer->m_extent;

  VkPipelineViewportStateCreateInfo viewport_state = {};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount = 1;
  viewport_state.pViewports = &viewport;
  viewport_state.scissorCount = 1;
  viewport_state.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer = {};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f; // Optional
  rasterizer.depthBiasClamp = 0.0f; // Optional
  rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

  VkPipelineMultisampleStateCreateInfo multisampling = {};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f; // Optional
  multisampling.pSampleMask = nullptr; // Optional
  multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
  multisampling.alphaToOneEnable = VK_FALSE; // Optional

  VkPipelineColorBlendAttachmentState color_blend_attachment = {};
  color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

  VkPipelineColorBlendStateCreateInfo color_blending = {};
  color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending.logicOpEnable = VK_FALSE;
  color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
  color_blending.attachmentCount = 1;
  color_blending.pAttachments = &color_blend_attachment;
  color_blending.blendConstants[0] = 0.0f; // Optional
  color_blending.blendConstants[1] = 0.0f; // Optional
  color_blending.blendConstants[2] = 0.0f; // Optional
  color_blending.blendConstants[3] = 0.0f; // Optional

  VkPipelineLayoutCreateInfo pipeline_layout_info = {};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.setLayoutCount = 1; // Optional
  pipeline_layout_info.pSetLayouts = &p_renderer->m_descriptor_set_layout; // Optional
  pipeline_layout_info.pushConstantRangeCount = 0; // Optional
  pipeline_layout_info.pPushConstantRanges = nullptr; // Optional


  if (vkCreatePipelineLayout(p_renderer->m_logical_device, 
                             &pipeline_layout_info, 
                             nullptr, 
                             &p_renderer->m_pipeline_layout) != VK_SUCCESS) 
  {
    log->error("failed to create pipeline layout!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  VkAttachmentDescription color_attachment = {};
  color_attachment.format = p_renderer->m_surface_format.format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref = {};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  VkRenderPassCreateInfo render_pass_info = {};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;

	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;

  if (vkCreateRenderPass(p_renderer->m_logical_device, 
                         &render_pass_info, 
                         nullptr, 
                         &p_renderer->m_render_pass) != VK_SUCCESS) 
  {
    log->error("failed to create render pass!");
    report_error(TNA_ERROR::E_SUCCESS);
  }

  VkGraphicsPipelineCreateInfo pipeline_info = {};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = 2;
  pipeline_info.pStages = shader_stages;
  pipeline_info.pVertexInputState = &vertex_infput_info;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterizer;
  pipeline_info.pMultisampleState = &multisampling;
  pipeline_info.pDepthStencilState = nullptr; // Optional
  pipeline_info.pColorBlendState = &color_blending;
  pipeline_info.pDynamicState = nullptr; // Optional
  pipeline_info.layout = p_renderer->m_pipeline_layout;
  pipeline_info.renderPass = p_renderer->m_render_pass;
  pipeline_info.subpass = 0;
  pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
  pipeline_info.basePipelineIndex = -1; // Optional
  pipeline_info.pNext = nullptr;

  if (vkCreateGraphicsPipelines(p_renderer->m_logical_device, 
                                VK_NULL_HANDLE, 
                                1, 
                                &pipeline_info, 
                                nullptr, 
                                &p_renderer->m_pipeline) != VK_SUCCESS) 
  {
    log->error("failed to create graphics pipeline!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  p_renderer->m_num_frame_buffers = p_renderer->m_num_swap_chain_image_views;
  p_renderer->m_frame_buffers = new VkFramebuffer[p_renderer->m_num_frame_buffers];

  for (size_t i = 0; i < p_renderer->m_num_frame_buffers; i++) 
  {
    VkImageView attachments[] = {
      p_renderer->m_swap_chain_image_views[i]
    };

    VkFramebufferCreateInfo frame_buffer_info = {};
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.renderPass = p_renderer->m_render_pass;
    frame_buffer_info.attachmentCount = 1;
    frame_buffer_info.pAttachments = attachments;
    frame_buffer_info.width = p_renderer->m_extent.width;
    frame_buffer_info.height = p_renderer->m_extent.height;
    frame_buffer_info.layers = 1;

    if (vkCreateFramebuffer(p_renderer->m_logical_device, 
                            &frame_buffer_info, 
                            nullptr, 
                            &p_renderer->m_frame_buffers[i]) != VK_SUCCESS) 
    {
      log->error("failed to create framebuffer!");
      report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
    }
  }

  shader_registry->unload("shaders/vert.spv");
  shader_registry->unload("shaders/frag.spv");
  return;
}

void
create_command_pool() 
{

  VkCommandPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_info.queueFamilyIndex = p_renderer->m_queue_indices.m_graphics_queue;
  pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

  if (vkCreateCommandPool(p_renderer->m_logical_device, 
                          &pool_info, 
                          nullptr, 
                          &p_renderer->m_command_pool) != VK_SUCCESS) 
  {
    log->error("failed to create command pool!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }
  return;
}

void 
create_descriptor_pool() 
{
  VkDescriptorPoolSize pool_size = {};
  pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
  pool_size.descriptorCount = p_renderer->m_num_swap_chain_images;

  VkDescriptorPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = &pool_size;
  pool_info.maxSets = p_renderer->m_num_swap_chain_images;

  if (vkCreateDescriptorPool(p_renderer->m_logical_device, 
                             &pool_info, 
                             nullptr, 
                             &p_renderer->m_descriptor_pool) != VK_SUCCESS) 
  {
    log->error("failed to create descriptor pool!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }
  return;
}

void
create_descriptor_set() 
{
  VkDescriptorSetLayoutBinding ubo_layout_info = {};
  ubo_layout_info.binding = 0;
  ubo_layout_info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
  ubo_layout_info.descriptorCount = 1;
  ubo_layout_info.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  ubo_layout_info.pImmutableSamplers = nullptr; // Optional

  VkDescriptorSetLayoutCreateInfo layout_info = {};
  layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layout_info.bindingCount = 1;
  layout_info.pBindings = &ubo_layout_info;

  if (vkCreateDescriptorSetLayout(p_renderer->m_logical_device, 
                                  &layout_info, 
                                  nullptr, 
                                  &p_renderer->m_descriptor_set_layout) != VK_SUCCESS) 
  {
    log->error("failed to create descriptor set layout!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  p_renderer->m_num_descriptor_sets = p_renderer->m_num_swap_chain_images;
  p_renderer->m_descriptor_sets = new VkDescriptorSet[p_renderer->m_num_descriptor_sets];

  VkDescriptorSetLayout* layouts = new VkDescriptorSetLayout[p_renderer->m_num_descriptor_sets];
  for(uint32_t i = 0; i < p_renderer->m_num_descriptor_sets; ++i)
  {
    layouts[i] = p_renderer->m_descriptor_set_layout;
  }

  VkDescriptorSetAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = p_renderer->m_descriptor_pool;
  allocInfo.descriptorSetCount = p_renderer->m_num_descriptor_sets;
  allocInfo.pSetLayouts = layouts;

  if (vkAllocateDescriptorSets(p_renderer->m_logical_device, 
                               &allocInfo, 
                               &p_renderer->m_descriptor_sets[0]) != VK_SUCCESS) 
  {
    log->error("failed to allocate descriptor set!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  delete [] layouts;

  return;
}

void 
create_uniform_buffers() 
{
  size_t device_alignment = p_renderer->m_mem_properties.limits.minUniformBufferOffsetAlignment;
  size_t uniform_buffer_size = sizeof(UniformBufferObject);
  size_t dynamic_alignment = (uniform_buffer_size / device_alignment) * device_alignment + ((uniform_buffer_size % device_alignment) > 0 ? device_alignment : 0);

  size_t bufferSize = dynamic_alignment * MAX_PRIMITIVE_COUNT;


  m_num_uniform_buffers = p_renderer->m_num_swap_chain_images;
  m_uniform_buffers = new VkBuffer[m_num_uniform_buffers];
  m_uniform_allocations = new VmaAllocation[m_num_uniform_buffers];

  for(uint32_t i = 0; i < m_num_uniform_buffers; ++i)
  {
    create_buffer(p_renderer->m_vkallocator,
                  bufferSize, 
                  VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                  VMA_MEMORY_USAGE_CPU_TO_GPU,
                  &m_uniform_buffers[i], 
                  &m_uniform_allocations[i]);

  }
  return;
}

void  
create_command_buffers() 
{
  p_renderer->m_num_command_buffers = p_renderer->m_num_frame_buffers;
  p_renderer->m_command_buffers = new VkCommandBuffer[p_renderer->m_num_command_buffers];

  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = p_renderer->m_command_pool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t) p_renderer->m_num_command_buffers;

  if (vkAllocateCommandBuffers(p_renderer->m_logical_device, 
                               &allocInfo, 
                               p_renderer->m_command_buffers) != VK_SUCCESS) 
  {
    log->error("failed to allocate command buffers!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }
  return;
}

void 
create_semaphores() 
{
  VkSemaphoreCreateInfo semaphore_info = {};
  semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(p_renderer->m_logical_device, 
                        &semaphore_info, 
                        nullptr, 
                        &p_renderer->m_image_available_semaphore) != VK_SUCCESS ||
			vkCreateSemaphore(p_renderer->m_logical_device, 
                        &semaphore_info, 
                        nullptr, 
                        &p_renderer->m_render_finished_semaphore) != VK_SUCCESS) 
  {
		log->error("failed to create semaphores!");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
	}
  return;
}


void 
clean_up_swap_chain() 
{
  if(p_renderer->m_swap_chain_images)
  {
    delete [] p_renderer->m_swap_chain_images;
    p_renderer->m_swap_chain_images = nullptr;
    p_renderer->m_num_swap_chain_images = 0;
  }

  vkDestroySwapchainKHR(p_renderer->m_logical_device, 
                        p_renderer->m_swap_chain, 
                        nullptr);

  return;
}

void
clean_up_image_views()
{
  if(p_renderer->m_swap_chain_image_views)
  {
    for(uint32_t i = 0; i < p_renderer->m_num_swap_chain_image_views; ++i)
    {
      vkDestroyImageView(p_renderer->m_logical_device, 
                         p_renderer->m_swap_chain_image_views[i], 
                         nullptr);
    }
    delete [] p_renderer->m_swap_chain_image_views;
    p_renderer->m_swap_chain_image_views = nullptr;
    p_renderer->m_num_swap_chain_image_views = 0;
  }
  return;
}

void
clean_up_graphics_pipeline()
{
  if(p_renderer->m_frame_buffers)
  {
    for (uint32_t i = 0; i < p_renderer->m_num_frame_buffers; ++i) 
    {
      vkDestroyFramebuffer(p_renderer->m_logical_device, 
                           p_renderer->m_frame_buffers[i], 
                           nullptr);
    }
    delete [] p_renderer->m_frame_buffers;
    p_renderer->m_frame_buffers = nullptr;
    p_renderer->m_num_frame_buffers = 0;
  }

  vkDestroyPipeline(p_renderer->m_logical_device, p_renderer->m_pipeline, nullptr);
  vkDestroyRenderPass(p_renderer->m_logical_device, p_renderer->m_render_pass, nullptr);
  vkDestroyPipelineLayout(p_renderer->m_logical_device, p_renderer->m_pipeline_layout, nullptr);
  return;
}

void 
recreate_swap_chain() 
{
  vkDeviceWaitIdle(p_renderer->m_logical_device);

  clean_up_graphics_pipeline();
  clean_up_image_views();
  clean_up_swap_chain();

  create_swap_chain();
  create_image_views();
  create_graphics_pipeline();
  create_command_buffers();

  return;
}

void 
build_command_buffer(uint32_t index) 
{
  m_view_matrix = m_scene.get_camera();
  m_projection_matrix = create_projection_matrix(60.0f, 
                                                 p_renderer->m_viewport_width / (float)(p_renderer->m_viewport_height),
                                                 0.1f,
                                                 10.0f);
  m_projection_matrix[1][1] *= -1;

  size_t device_alignment = p_renderer->m_mem_properties.limits.minUniformBufferOffsetAlignment;
  size_t uniform_buffer_size = sizeof(UniformBufferObject);
  size_t dynamic_alignment = (uniform_buffer_size / device_alignment) * device_alignment + ((uniform_buffer_size % device_alignment) > 0 ? device_alignment : 0);
  
  const VkRenderingInfo* meshes;
  uint32_t num_meshes;
  m_scene.get_meshes(&meshes, &num_meshes);
  if(num_meshes > 0)
  {
    for(size_t i = 0; i < num_meshes && i < MAX_PRIMITIVE_COUNT; ++i) 
    {
      const VkRenderingInfo* info = &meshes[i];
      UniformBufferObject ubo;
      ubo.projmodelview = m_projection_matrix * m_view_matrix * info->m_model_mat;

      void* uniform_data = nullptr;
      vmaMapMemory(p_renderer->m_vkallocator, m_uniform_allocations[index], &uniform_data);
      memcpy(&(((char*)uniform_data)[i*dynamic_alignment]), &ubo, sizeof(ubo)); 
      vmaUnmapMemory(p_renderer->m_vkallocator, m_uniform_allocations[index]);
      //vmaFlushAllocation(p_renderer->m_vkallocator, m_uniform_allocations[index], i*dynamic_alignment, sizeof(ubo));
    }

    /// Update descriptor set
    VkDescriptorBufferInfo buffer_info = {};
    buffer_info.buffer = m_uniform_buffers[index];
    buffer_info.offset = 0;
    buffer_info.range = dynamic_alignment*num_meshes;

    VkWriteDescriptorSet descriptor_write = {};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.pNext = nullptr;
    descriptor_write.dstSet = p_renderer->m_descriptor_sets[index];
    descriptor_write.dstBinding = 0;
    descriptor_write.dstArrayElement = 0;

    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo = &buffer_info;
    descriptor_write.pImageInfo = nullptr; // Optional
    descriptor_write.pTexelBufferView = nullptr; // Optional

    vkUpdateDescriptorSets(p_renderer->m_logical_device, 
                           1, 
                           &descriptor_write, 
                           0, 
                           nullptr);
  }

  /// Setting up command buffer
  VkCommandBufferBeginInfo begin_info = {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  begin_info.pInheritanceInfo = nullptr; // Optional

  if (vkBeginCommandBuffer(p_renderer->m_command_buffers[index], &begin_info) != VK_SUCCESS) 
  {
    log->error("failed to begin recording command buffer!");
    report_error(TNA_ERROR::E_RENDERER_RUNTIME_ERROR);
  }

  vkCmdBindPipeline(p_renderer->m_command_buffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, p_renderer->m_pipeline);

  VkRenderPassBeginInfo renderpass_info = {};
  renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderpass_info.renderPass = p_renderer->m_render_pass;
  renderpass_info.framebuffer = p_renderer->m_frame_buffers[index];
  renderpass_info.renderArea.offset = {0, 0};
  renderpass_info.renderArea.extent = p_renderer->m_extent;

  VkClearValue clear_color = {0.0f, 0.0f, 1.0f, 1.0f};
  renderpass_info.clearValueCount = 1;
  renderpass_info.pClearValues = &clear_color;

  vkCmdBeginRenderPass(p_renderer->m_command_buffers[index], &renderpass_info, VK_SUBPASS_CONTENTS_INLINE);

  for(size_t i = 0; i < num_meshes && i < MAX_PRIMITIVE_COUNT; ++i) 
  {
    const VkRenderingInfo* info = &meshes[i];
    VkBuffer vertex_buffers[] = {info->m_mesh_data->m_vertex_buffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(p_renderer->m_command_buffers[index], 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(p_renderer->m_command_buffers[index], info->m_mesh_data->m_index_buffer, 0, VK_INDEX_TYPE_UINT32);
    uint32_t offset = dynamic_alignment*i;
    vkCmdBindDescriptorSets(p_renderer->m_command_buffers[index], 
                            VK_PIPELINE_BIND_POINT_GRAPHICS, 
                            p_renderer->m_pipeline_layout, 
                            0, 
                            1, 
                            &p_renderer->m_descriptor_sets[index], 
                            1, 
                            &offset);

    vkCmdDrawIndexed(p_renderer->m_command_buffers[index], info->m_mesh_data->m_num_indices, 1, 0, 0, 0);
  }

  vkCmdEndRenderPass(p_renderer->m_command_buffers[index]);

  if (vkEndCommandBuffer(p_renderer->m_command_buffers[index]) != VK_SUCCESS) 
  {
    log->error("failed to record command buffer!");
    report_error(TNA_ERROR::E_RENDERER_RUNTIME_ERROR);
  }
  return;
}


////////////////////////////////////////////////
////////////// PUBLIC METHODS //////////////////
////////////////////////////////////////////////

void 
init_renderer(const Config* config, 
              GLFWwindow* window) 
{
  p_renderer = new VulkanRenderer();
  
  // Retrieving vulkan validation layers from engine config
  p_renderer->m_num_validation_layers = config->m_num_vk_vlayers;
  p_renderer->m_validation_layers = new const char*[p_renderer->m_num_validation_layers];
  for(uint32_t i = 0; i < p_renderer->m_num_validation_layers; ++i) 
  {
    const char* layer = (config->m_vk_vlayers[i])->c_str();
    p_renderer->m_validation_layers[i] = new char[strlen(layer)+1];
    strcpy((char*)p_renderer->m_validation_layers[i], layer);
  }


  // Retrieving viewport dimensions from engine config
  p_renderer->m_viewport_width = config->m_viewport_width;
  p_renderer->m_viewport_height = config->m_viewport_height;

  if(enable_validation_layers)
  {
    log->warning("Vulkan validation layers loaded");
  }

  create_vulkan_instance();
  create_surface(window);
  add_debug_handler();
  create_physical_device();
  create_logical_device();

  // Creating memory allocator
  VmaAllocatorCreateInfo allocator_info = {};
  allocator_info.physicalDevice = p_renderer->m_physical_device;
  allocator_info.device = p_renderer->m_logical_device;
  vmaCreateAllocator(&allocator_info, &p_renderer->m_vkallocator);

  create_command_queues();

  create_command_pool();

  create_swap_chain();

  create_image_views();
  
  create_descriptor_pool();

  create_descriptor_set();

  create_graphics_pipeline();

  create_uniform_buffers();

  create_command_buffers();

  create_semaphores();

  return;
}


void 
terminate_renderer() 
{
  vkDeviceWaitIdle(p_renderer->m_logical_device);

  shader_registry->clear();
  mesh_registry->clear();

  for(uint32_t i = 0; i < m_num_uniform_buffers; ++i)
  {
    destroy_buffer(p_renderer->m_vkallocator,
                   m_uniform_buffers[i],
                   m_uniform_allocations[i]);
  }
  delete [] m_uniform_buffers;
  delete [] m_uniform_allocations;

  delete p_renderer;

  return;
}


void
begin_frame() 
{
  m_scene.clear();
  return;
}

void 
end_frame() 
{
	uint32_t image_index;
	VkResult result = vkAcquireNextImageKHR(p_renderer->m_logical_device, 
                                          p_renderer->m_swap_chain, 
                                          std::numeric_limits<uint64_t>::max(), 
                                          p_renderer->m_image_available_semaphore, 
                                          VK_NULL_HANDLE, 
                                          &image_index);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) 
  {
    recreate_swap_chain();
    return;
  } 
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
  {
    log->error("failed to acquire swap chain image!");
    report_error(TNA_ERROR::E_RENDERER_RUNTIME_ERROR);
  }

  build_command_buffer(image_index);

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore wait_semaphores[] = {p_renderer->m_image_available_semaphore};
	VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphores;
	submit_info.pWaitDstStageMask = wait_stages;

  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &p_renderer->m_command_buffers[image_index];

  VkSemaphore signal_semaphores[] = {p_renderer->m_render_finished_semaphore};
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  if (vkQueueSubmit(p_renderer->m_graphics_queue, 
                    1, 
                    &submit_info, 
                    VK_NULL_HANDLE) != VK_SUCCESS) 
  {
    log->error("failed to submit draw command buffer!");
    report_error(TNA_ERROR::E_RENDERER_RUNTIME_ERROR);
  }

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signal_semaphores;

	VkSwapchainKHR swapChains[] = {p_renderer->m_swap_chain};
	present_info.swapchainCount = 1;
	present_info.pSwapchains = swapChains;
	present_info.pImageIndices = &image_index;
	present_info.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(p_renderer->m_present_queue, 
                             &present_info);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) 
  {
    recreate_swap_chain();
  } 
  else if (result != VK_SUCCESS) 
  {
    log->error("failed to present swap chain image!");
    report_error(TNA_ERROR::E_RENDERER_RUNTIME_ERROR);
  }

  vkQueueWaitIdle(p_renderer->m_present_queue);
  return;
}

void 
render_mesh(const MeshData* mesh_data, 
            const Matrix4* model_mat ) 
{

  m_scene.add_mesh(static_cast<const VkMeshData*>(mesh_data),
                   model_mat);
  return;
}

void 
set_camera(const Matrix4* camera_mat) 
{
  m_scene.set_camera(camera_mat);
  return;
}

}
