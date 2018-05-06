


#include "../../common.h"
#include "../../config.h"
#include "../../resources/resources.h"
#include "../../tools/files.h"
#include "../vertex.h"


#define VMA_IMPLEMENTATION
#include "vkmem_alloc.h"

#include "vkbuffer_tools.h"
#include "vkmesh_data.h"
#include "vkscene.h"
#include "vkshader.h"
#include "vkshader_tools.h"
#include "vkvertex_tools.h"
#include <array>
#include <cstring>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <limits>
#include <set>
#include <chrono>

namespace tna {
namespace rendering {

#define MAX_PRIMITIVE_COUNT 65536

/**
 * @brief Structure used to store the indices to the different families of
 * available queues
 */
struct QueueFamilyIndices {

	int32_t m_graphics_queue = -1;
  int32_t m_present_queue = -1;

  void reset(){
    m_graphics_queue = -1;
    m_present_queue = -1;
  }

  bool is_complete() {
    return  m_graphics_queue != -1 &&
            m_present_queue != -1;
  }
	
};

/**
 * @brief Struct used to store the Swap Chain capabilities and support details
 */
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR        m_capabilities;
  std::vector<VkSurfaceFormatKHR> m_formats;
  std::vector<VkPresentModeKHR>   m_present_modes;
};


/**
 * @brief Width of the viewport
 */
uint32_t                  m_viewport_width = 0;

/**
 * @brief Height of the viewport
 */
uint32_t                  m_viewport_height = 0;

/**
 * @brief Vulkan instance
 */
VkInstance                m_vulkan_instance = VK_NULL_HANDLE;

/**
 * @brief Rendering surface
 */
VkSurfaceKHR              m_window_surface = VK_NULL_HANDLE;


/**
 * @brief Debug report callback
 */
VkDebugReportCallbackEXT  m_report_callback = VK_NULL_HANDLE;

/**
 * @brief Physical device
 */
VkPhysicalDevice          m_physical_device = VK_NULL_HANDLE;


VkPhysicalDeviceProperties m_mem_properties;


/**
 * @brief Logical device
 */
VkDevice                  m_logical_device = VK_NULL_HANDLE;

/**
 * @brief Rendering Surface format
 */
VkSurfaceFormatKHR        m_surface_format;


/**
 * @brief Rendering Surface extent
 */
VkExtent2D                m_extent;

/**
 * @brief Swap chain
 */
VkSwapchainKHR            m_swap_chain = VK_NULL_HANDLE;


/**
 * @brief Vector with the different swap chain images
 */
std::vector<VkImage>      m_swap_chain_images;

/**
 * @brief Vector with the different swap chain image views
 */
std::vector<VkImageView>  m_swap_chain_image_views;


/**
 * @brief pipeline layout
 */
VkPipelineLayout          m_pipeline_layout;


/**
 * @brief Render pass object
 */
VkRenderPass              m_render_pass;


/**
 * @brief Rendering pipeline
 */
VkPipeline                m_pipeline;


/**
 * @brief Rendering command pool
 */
VkCommandPool             m_command_pool;

/**
 * @brief The descriptor pool
 */
VkDescriptorPool          m_descriptor_pool;

/**
 * @brief The descriptor set of the pipeline
 */
VkDescriptorSet           m_descriptor_set;

/**
 * @brief Semaphore for synchronizing next frame image retrieval
 */
VkSemaphore               m_image_available_semaphore;

/**
 * @brief Sempahore to synchronize render pass finished
 */
VkSemaphore               m_render_finished_semaphore;

/**
 * @brief Queue family indices
 */
QueueFamilyIndices 			  m_queue_indices;


/**
 * @brief Graphics command queue
 */
VkQueue                   m_graphics_queue;


/**
 * @brief Present command queue
 */
VkQueue                   m_present_queue;

/**
 * @brief Vector with the different validation layers to load. Filled up
 * dynamically from the config file
 */
std::vector<const char*>        m_validation_layers;


/**
 * @brief Vector with the required physical device extensions
 */
const std::vector<const char*>  m_device_extensions = {
                                                    VK_KHR_SWAPCHAIN_EXTENSION_NAME
                                                   };

/**
 * @brief Vector with the different frame buffers. One per swap chain image
 */
std::vector<VkFramebuffer>      m_frame_buffers;


/**
 * @brief Vector with the different command buffers. One per swap chain image 
 */
std::vector<VkCommandBuffer>    m_command_buffers;


/**
 * @brief Flag used to enable validation layers or not
 */
#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif


/**
 * @brief The memory allocator used to create Vulkan buffers
 */
VmaAllocator                    m_vkallocator;


/**
 * @brief The Vulkan Scene object to store the scene to render information
 */
VkScene                         m_scene;

glm::mat4                       m_projection_matrix;

glm::mat4                       m_view_matrix;

struct UniformBufferObject {
    glm::mat4 projmodelview;
};


VkDescriptorSetLayout m_descriptor_set_layout;
VkBuffer              m_uniform_buffer;
VmaAllocation         m_uniform_allocation;
void*                 m_udata = nullptr;

/**
 * @brief Queries the physical device for the actual swap chain support
 *
 * @param device The physical device to query the support from
 * @param surface The rendering surface
 *
 * @return Returns a SwapChainSupportDetails structure with the given available
 * support
 */
static SwapChainSupportDetails query_sc_support_details(VkPhysicalDevice device, 
                                                        VkSurfaceKHR surface) {

  SwapChainSupportDetails sc_support_details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, 
                                            surface, 
                                            &sc_support_details.m_capabilities);
  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, 
                                       surface, 
                                       &format_count, 
                                       nullptr);
  if (format_count != 0) {
    sc_support_details.m_formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, 
                                         surface, 
                                         &format_count, 
                                         sc_support_details.m_formats.data());
  }

  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, 
                                            surface, 
                                            &present_mode_count, 
                                            nullptr);
  if (present_mode_count != 0) {
    sc_support_details.m_present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, 
                                              surface, 
                                              &present_mode_count, 
                                              sc_support_details.m_present_modes.data());
  }
  return sc_support_details;
}

/**
 * @brief Given the different available surface formats for the swap chain,
 * choses the best one
 *
 * @param available_formats A vector with the available surface formats
 *
 * @return Returns the chosen surface format
 */
static VkSurfaceFormatKHR choose_sc_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) {
  if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED) {
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto& available_format : available_formats) {
    if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM && 
        available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return available_format;
    }
  }

  return available_formats[0];
}

/**
 * @brief Given the different available present modes, choses the best one
 *
 * @param available_present_modes A vector with the available present modes
 *
 * @return The choosen present mode
 */
static VkPresentModeKHR choose_sc_present_mode(const std::vector<VkPresentModeKHR> available_present_modes) {
    VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto& available_present_mode : available_present_modes) {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return available_present_mode;
        } else if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            best_mode = available_present_mode;
        }
    }

    return best_mode;
}

/**
 * @brief Chooses the extent of the swap chain
 *
 * @param capabilities The capabilities of the surface
 * @param width The width of the viewport 
 * @param height The height of the viewport 
 *
 * @return Returns the choosen extent
 */
static VkExtent2D choose_sc_extent(const VkSurfaceCapabilitiesKHR& capabilities, 
                                   uint32_t width,
                                   uint32_t height) {

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actual_extent = {width, height};

        actual_extent.width = std::max(capabilities.minImageExtent.width, 
                                       std::min(capabilities.maxImageExtent.width, 
                                                actual_extent.width)
                                       );

        actual_extent.height = std::max(capabilities.minImageExtent.height, 
                                        std::min(capabilities.maxImageExtent.height, 
                                                 actual_extent.height)
                                        );

        return actual_extent;
    }

}


/**
 * @brief Callback used to capture debug information
 *
 * @param flags
 * @param objType
 * @param obj
 * @param location
 * @param code
 * @param layerPrefix
 * @param msg
 * @param userData
 *
 * @return 
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
                                        VkDebugReportFlagsEXT flags,
                                        VkDebugReportObjectTypeEXT objType,
                                        uint64_t obj,
                                        size_t location,
                                        int32_t code,
                                        const char* layerPrefix,
                                        const char* msg,
                                        void* userData) {
  log->error("validation layer: %s", msg);
  return VK_FALSE;
}


/**
 * @brief Checks the support of the physical device for the desired extensions
 *
 * @param device The physical device to check the extensions support of
 * @param desired_extensions The extensions wanted to check
 *
 * @return Returns true if all the desired extensions are supported
 */
bool check_device_extension_support(VkPhysicalDevice device,
                                    const std::vector<const char*>& desired_extensions) {

    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    std::set<std::string> required_extensions(desired_extensions.begin(), 
                                              desired_extensions.end());

    for (const auto& extension : available_extensions) {
      log->log("Found Vulkan device extension %s", extension.extensionName);
      required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

/**
 * @brief Checks if the given physical device is suitable for our application
 *
 * @param device The physical device to check for
 * @param surface The rendering surface 
 *
 * @return Returns true if the device is suitable
 */
static bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface) {

  bool extensions_supported = check_device_extension_support(device, m_device_extensions);

  bool swap_chain_adequate = false;
  if(extensions_supported) {
    SwapChainSupportDetails sc_support_details = query_sc_support_details(device, surface);
    swap_chain_adequate = !sc_support_details.m_formats.empty() && !sc_support_details.m_present_modes.empty();
  }

  return m_queue_indices.is_complete() && extensions_supported && swap_chain_adequate;
}

static void create_vulkan_instance() {

  // Getting Available Validation Layers
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()); 

  for (const char* layer : m_validation_layers) {
    bool layer_found = false;
    for (const auto& layerProperties : available_layers) {
      if (strcmp(layer, layerProperties.layerName) == 0) {
        log->log("Found validation layer %s", layer);
        layer_found = true;
        break;
      }
    }

    if (!layer_found) {
      throw std::runtime_error("Validation layer "+std::string(layer)+" not found");
    }
  }

  // Creating Vulkan Instance info
  VkApplicationInfo app_info = {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "TNA";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "TNA Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  if(enable_validation_layers) {
    create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
    create_info.ppEnabledLayerNames = m_validation_layers.data();
  }

  // Adding GLFW and other required extensions
  std::vector<const char*> extensions;
  unsigned int glfw_extensions_count = 0;
  const char** glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extensions_count);
  for(uint32_t i = 0; i < glfw_extensions_count; ++i) {
    extensions.push_back(glfw_extensions[i]);
  }

  if(enable_validation_layers) {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();


  if(vkCreateInstance(&create_info, 
                      nullptr, 
                      &m_vulkan_instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

static void create_surface(GLFWwindow* window) {

  if (glfwCreateWindowSurface(m_vulkan_instance, 
                              window, 
                              nullptr, 
                              &m_window_surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }

}

static void add_debug_handler() {
  // Adding debug callback
  if(enable_validation_layers) {
    VkDebugReportCallbackCreateInfoEXT debug_callback_info = {};
    debug_callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    debug_callback_info.pfnCallback = debug_callback;

    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(m_vulkan_instance, "vkCreateDebugReportCallbackEXT");
    if(func(m_vulkan_instance, &debug_callback_info, nullptr, &m_report_callback) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create debug callback!");
    }
  }
}

static void create_physical_device() {

	// Creating Physical Device
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(m_vulkan_instance, 
                             &device_count, 
                             nullptr);

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(m_vulkan_instance, 
                             &device_count, 
                             devices.data());

  for (const auto& device : devices) {
    m_queue_indices.reset();

		uint32_t queue_familty_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, 
                                             &queue_familty_count, 
                                             nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(queue_familty_count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, 
                                             &queue_familty_count, 
                                             queue_families.data());

		int i = 0;
		for (const auto& queue_family : queue_families) {
			if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				m_queue_indices.m_graphics_queue = i;
			}

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_window_surface, &presentSupport);
      if(presentSupport) {
        m_queue_indices.m_present_queue = i;
      }

			i++;
		}

    if (is_device_suitable(device, m_window_surface)) {
      m_physical_device = device;
      break;
    }
  }


  if (m_physical_device == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }

  vkGetPhysicalDeviceProperties(m_physical_device, &m_mem_properties);
}

static void create_logical_device() {

  // Creating logical devie
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<int> unique_queue_families = {m_queue_indices.m_graphics_queue, 
                                         m_queue_indices.m_present_queue};
  float queue_priority = 1.0f;

  for(int queue_family : unique_queue_families) {
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkPhysicalDeviceFeatures device_features = {};

  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = queue_create_infos.data();
  createInfo.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  createInfo.pEnabledFeatures = &device_features;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(m_device_extensions.size());
  createInfo.ppEnabledExtensionNames = m_device_extensions.data();

  if (enable_validation_layers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
    createInfo.ppEnabledLayerNames = m_validation_layers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(m_physical_device, 
                     &createInfo, 
                     nullptr, 
                     &m_logical_device) != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }
}

static void create_command_queues() {
  // Retrieving queues
  vkGetDeviceQueue(m_logical_device, m_queue_indices.m_graphics_queue, 0, &m_graphics_queue);
  vkGetDeviceQueue(m_logical_device, m_queue_indices.m_present_queue, 0, &m_present_queue);
}

static void create_swap_chain() {

  // Creating Swap chain
  SwapChainSupportDetails swap_chain_support = query_sc_support_details(m_physical_device, 
                                                                        m_window_surface);

  m_surface_format = choose_sc_surface_format(swap_chain_support.m_formats);

  VkPresentModeKHR present_mode = choose_sc_present_mode(swap_chain_support.m_present_modes);

  m_extent = choose_sc_extent(swap_chain_support.m_capabilities, 
                                       m_viewport_width, 
                                       m_viewport_height);

  uint32_t image_count = swap_chain_support.m_capabilities.minImageCount + 1;
  if (swap_chain_support.m_capabilities.maxImageCount > 0 && 
      image_count > swap_chain_support.m_capabilities.maxImageCount) {
    image_count = swap_chain_support.m_capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR swap_chain_create_info = {};
  swap_chain_create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swap_chain_create_info.surface          = m_window_surface;
  swap_chain_create_info.minImageCount    = image_count;
  swap_chain_create_info.imageFormat      = m_surface_format.format;
  swap_chain_create_info.imageColorSpace  = m_surface_format.colorSpace;
  swap_chain_create_info.imageExtent      = m_extent;
  swap_chain_create_info.imageArrayLayers = 1;
  swap_chain_create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  uint32_t queue_family_indices[] = {(uint32_t) m_queue_indices.m_graphics_queue, 
                                    (uint32_t) m_queue_indices.m_present_queue};

  if (m_queue_indices.m_graphics_queue != m_queue_indices.m_present_queue) {
    swap_chain_create_info.imageSharingMode       = VK_SHARING_MODE_CONCURRENT;
    swap_chain_create_info.queueFamilyIndexCount  = 2;
    swap_chain_create_info.pQueueFamilyIndices    = queue_family_indices;
  } else {
    swap_chain_create_info.imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE;
    swap_chain_create_info.queueFamilyIndexCount  = 0; // Optional
    swap_chain_create_info.pQueueFamilyIndices    = nullptr; // Optional
  }

  swap_chain_create_info.preTransform   = swap_chain_support.m_capabilities.currentTransform;
  swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swap_chain_create_info.presentMode    = present_mode;
  swap_chain_create_info.clipped        = VK_TRUE;

  swap_chain_create_info.oldSwapchain   = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(m_logical_device, 
                           &swap_chain_create_info, 
                           nullptr, 
                           &m_swap_chain) != VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(m_logical_device, m_swap_chain, &image_count, nullptr);
  m_swap_chain_images.resize(image_count);
  vkGetSwapchainImagesKHR(m_logical_device, m_swap_chain, &image_count, m_swap_chain_images.data());

}

static void create_image_views() {
  m_swap_chain_image_views.resize(m_swap_chain_images.size());
  for(size_t i = 0; i < m_swap_chain_image_views.size(); ++i) {

    VkImageViewCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = m_swap_chain_images[i];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = m_surface_format.format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(m_logical_device, 
                          &create_info, 
                          nullptr, 
                          &m_swap_chain_image_views[i]) != VK_SUCCESS) {

      throw std::runtime_error("failed to create image views!");
    }

  }
}

static void create_graphics_pipeline() {



  optional<Shader*> vertex_shader = resources::shader_registry->load("shaders/vert.spv");
  optional<Shader*> fragment_shader = resources::shader_registry->load("shaders/frag.spv");
  
  if(!vertex_shader) {
    throw std::runtime_error("Vertex shader not found");
  }

  if(!fragment_shader) {
    throw std::runtime_error("Fragment shader not found");
  }

  VkShader* vkvertex_shader = static_cast<VkShader*>(vertex_shader.get());
  VkShader* vkfragment_shader = static_cast<VkShader*>(fragment_shader.get());

  VkPipelineShaderStageCreateInfo vertex_shader_stage = build_vertex_shader_stage(vkvertex_shader->m_shader_module);
  VkPipelineShaderStageCreateInfo fragment_shader_stage = build_fragment_shader_stage(vkfragment_shader->m_shader_module);

  VkPipelineShaderStageCreateInfo shader_stages[] = {vertex_shader_stage, fragment_shader_stage};

  VkVertexInputBindingDescription binding_description = get_binding_description();
  std::array<VkVertexInputAttributeDescription,3> attribute_descriptions = get_attribute_descriptions();

  VkPipelineVertexInputStateCreateInfo vertex_infput_info = {};
  vertex_infput_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_infput_info.vertexBindingDescriptionCount = 1;
  vertex_infput_info.pVertexBindingDescriptions = &binding_description; // Optional
  vertex_infput_info.vertexAttributeDescriptionCount = attribute_descriptions.size();
  vertex_infput_info.pVertexAttributeDescriptions = attribute_descriptions.data(); // Optional

  VkPipelineInputAssemblyStateCreateInfo input_assembly = {};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float) m_extent.width;
  viewport.height = (float) m_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor = {};
  scissor.offset = {0, 0};
  scissor.extent = m_extent;


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
  pipeline_layout_info.pSetLayouts = &m_descriptor_set_layout; // Optional
  pipeline_layout_info.pushConstantRangeCount = 0; // Optional
  pipeline_layout_info.pPushConstantRanges = nullptr; // Optional


  if (vkCreatePipelineLayout(m_logical_device, &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }

  VkAttachmentDescription color_attachment = {};
  color_attachment.format = m_surface_format.format;
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

  if (vkCreateRenderPass(m_logical_device, &render_pass_info, nullptr, &m_render_pass) != VK_SUCCESS) {
    throw std::runtime_error("failed to create render pass!");
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
  pipeline_info.layout = m_pipeline_layout;
  pipeline_info.renderPass = m_render_pass;
  pipeline_info.subpass = 0;
  pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
  pipeline_info.basePipelineIndex = -1; // Optional

  if (vkCreateGraphicsPipelines(m_logical_device, 
                                VK_NULL_HANDLE, 
                                1, 
                                &pipeline_info, 
                                nullptr, 
                                &m_pipeline) != VK_SUCCESS) {
    throw std::runtime_error("failed to create graphics pipeline!");
  }


  m_frame_buffers.resize(m_swap_chain_image_views.size());

  for (size_t i = 0; i < m_swap_chain_image_views.size(); i++) {
    VkImageView attachments[] = {
      m_swap_chain_image_views[i]
    };

    VkFramebufferCreateInfo frame_buffer_info = {};
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.renderPass = m_render_pass;
    frame_buffer_info.attachmentCount = 1;
    frame_buffer_info.pAttachments = attachments;
    frame_buffer_info.width = m_extent.width;
    frame_buffer_info.height = m_extent.height;
    frame_buffer_info.layers = 1;

    if (vkCreateFramebuffer(m_logical_device, 
                            &frame_buffer_info, 
                            nullptr, 
                            &m_frame_buffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }

  resources::shader_registry->unload("shaders/vert.spv");
  resources::shader_registry->unload("shaders/frag.spv");

}

static void create_command_pool() {


  VkCommandPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_info.queueFamilyIndex = m_queue_indices.m_graphics_queue;
  pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

  if (vkCreateCommandPool(m_logical_device, 
                          &pool_info, 
                          nullptr, 
                          &m_command_pool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }

}

static void create_descriptor_pool() {

  VkDescriptorPoolSize pool_size = {};
  pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
  pool_size.descriptorCount = 1;

  VkDescriptorPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = &pool_size;

  pool_info.maxSets = 1;

  if (vkCreateDescriptorPool(m_logical_device, &pool_info, nullptr, &m_descriptor_pool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

static void create_descriptor_set() {

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


  if (vkCreateDescriptorSetLayout(m_logical_device, &layout_info, nullptr, &m_descriptor_set_layout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor set layout!");
  }

  VkDescriptorSetLayout layouts[] = {m_descriptor_set_layout};
  VkDescriptorSetAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = m_descriptor_pool;
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts = layouts;

  if (vkAllocateDescriptorSets(m_logical_device, &allocInfo, &m_descriptor_set) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate descriptor set!");
  }

}

static void create_uniform_buffers() {

  size_t device_alignment = m_mem_properties.limits.minUniformBufferOffsetAlignment;
  size_t uniform_buffer_size = sizeof(UniformBufferObject);
  size_t dynamic_alignment = (uniform_buffer_size / device_alignment) * device_alignment + ((uniform_buffer_size % device_alignment) > 0 ? device_alignment : 0);

  size_t bufferSize = dynamic_alignment * MAX_PRIMITIVE_COUNT;

  create_buffer(bufferSize, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                VMA_MEMORY_USAGE_CPU_TO_GPU,
                m_uniform_buffer, 
                m_uniform_allocation);


  vmaMapMemory(m_vkallocator, m_uniform_allocation, &m_udata);
}

static void create_command_buffers() {

  m_command_buffers.resize(m_frame_buffers.size());

  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = m_command_pool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t) m_command_buffers.size();

  if (vkAllocateCommandBuffers(m_logical_device, 
                               &allocInfo, 
                               m_command_buffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }


}

static void create_semaphores() {

  VkSemaphoreCreateInfo semaphore_info = {};
  semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(m_logical_device, &semaphore_info, nullptr, &m_image_available_semaphore) != VK_SUCCESS ||
			vkCreateSemaphore(m_logical_device, &semaphore_info, nullptr, &m_render_finished_semaphore) != VK_SUCCESS) {

		throw std::runtime_error("failed to create semaphores!");
	}


}


static void clean_up_swap_chain() {

  for (auto framebuffer : m_frame_buffers) {
    vkDestroyFramebuffer(m_logical_device, framebuffer, nullptr);
  }

  vkDestroyPipeline(m_logical_device, m_pipeline, nullptr);
  vkDestroyRenderPass(m_logical_device, m_render_pass, nullptr);
  vkDestroyPipelineLayout(m_logical_device, m_pipeline_layout, nullptr);

  for (auto imageView : m_swap_chain_image_views) {
    vkDestroyImageView(m_logical_device, 
                       imageView, 
                       nullptr);
  }

  vkDestroySwapchainKHR(m_logical_device, 
                        m_swap_chain, 
                        nullptr);
}

static void recreate_swap_chain() {

  vkDeviceWaitIdle(m_logical_device);

  clean_up_swap_chain();

  create_swap_chain();
  create_image_views();
  create_graphics_pipeline();
  create_command_buffers();
}

static void build_command_buffer(uint32_t index) {

  m_view_matrix = m_scene.get_camera();
  m_projection_matrix = glm::perspective(glm::radians(45.0f), m_extent.width / (float) m_extent.height, 0.1f, 10.0f);
  m_projection_matrix[1][1] *= -1;

  size_t device_alignment = m_mem_properties.limits.minUniformBufferOffsetAlignment;
  size_t uniform_buffer_size = sizeof(UniformBufferObject);
  size_t dynamic_alignment = (uniform_buffer_size / device_alignment) * device_alignment + ((uniform_buffer_size % device_alignment) > 0 ? device_alignment : 0);
  
  char* uniform_data = static_cast<char*>(m_udata);
  auto meshes = m_scene.get_meshes();
  for(size_t i = 0; i < meshes.size() && i < MAX_PRIMITIVE_COUNT; ++i) {
    auto info = meshes[i];
    UniformBufferObject ubo;
    ubo.projmodelview = m_projection_matrix * m_scene.get_camera() * info.m_model_mat;
    memcpy(&uniform_data[i*dynamic_alignment], &ubo, sizeof(ubo)); 
  }

  /// Update descriptor set
  VkDescriptorBufferInfo buffer_info = {};
  buffer_info.buffer = m_uniform_buffer;
  buffer_info.offset = 0;
  buffer_info.range = dynamic_alignment*meshes.size();

  VkWriteDescriptorSet descriptor_write = {};
  descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptor_write.dstSet = m_descriptor_set;
  descriptor_write.dstBinding = 0;
  descriptor_write.dstArrayElement = 0;

  descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
  descriptor_write.descriptorCount = 1;

  descriptor_write.pBufferInfo = &buffer_info;
  descriptor_write.pImageInfo = nullptr; // Optional
  descriptor_write.pTexelBufferView = nullptr; // Optional

  vkUpdateDescriptorSets(m_logical_device, 1, &descriptor_write, 0, nullptr);

  /// Setting up command buffer
  VkCommandBufferBeginInfo begin_info = {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  begin_info.pInheritanceInfo = nullptr; // Optional

  if (vkBeginCommandBuffer(m_command_buffers[index], &begin_info) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  vkCmdBindPipeline(m_command_buffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

  VkRenderPassBeginInfo renderpass_info = {};
  renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderpass_info.renderPass = m_render_pass;
  renderpass_info.framebuffer = m_frame_buffers[index];
  renderpass_info.renderArea.offset = {0, 0};
  renderpass_info.renderArea.extent = m_extent;

  VkClearValue clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
  renderpass_info.clearValueCount = 1;
  renderpass_info.pClearValues = &clear_color;

  vkCmdBeginRenderPass(m_command_buffers[index], &renderpass_info, VK_SUBPASS_CONTENTS_INLINE);


  for(size_t i = 0; i < meshes.size() && i < MAX_PRIMITIVE_COUNT; ++i) {
    auto info = meshes[i];
    VkBuffer vertex_buffers[] = {info.m_mesh_data->m_vertex_buffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(m_command_buffers[index], 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(m_command_buffers[index], info.m_mesh_data->m_index_buffer, 0, VK_INDEX_TYPE_UINT32);
    uint32_t offset = dynamic_alignment*i;
    vkCmdBindDescriptorSets(m_command_buffers[index], 
                            VK_PIPELINE_BIND_POINT_GRAPHICS, 
                            m_pipeline_layout, 0, 1, &m_descriptor_set, 1, &offset);

    vkCmdDrawIndexed(m_command_buffers[index], info.m_mesh_data->m_num_indices, 1, 0, 0, 0);
  }

  vkCmdEndRenderPass(m_command_buffers[index]);

  if (vkEndCommandBuffer(m_command_buffers[index]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}


////////////////////////////////////////////////
////////////// PUBLIC METHODS //////////////////
////////////////////////////////////////////////

void init_renderer(const Config& config, 
                    GLFWwindow* window) {

  for(auto& layer : config.m_vk_validation_layers) {
    char* str = new char[strlen(layer.c_str())+1];
    std::strcpy(str, layer.c_str());
    m_validation_layers.push_back(str);
  }
  m_viewport_width = config.m_viewport_width;
  m_viewport_height = config.m_viewport_height;

  if(enable_validation_layers) {
    log->warning("Vulkan validation layers loaded");
  }

  create_vulkan_instance();
  create_surface(window);
  add_debug_handler();
  create_physical_device();
  create_logical_device();

  VmaAllocatorCreateInfo allocator_info = {};
  allocator_info.physicalDevice = m_physical_device;
  allocator_info.device = m_logical_device;
  vmaCreateAllocator(&allocator_info, &m_vkallocator);

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
}


void terminate_renderer() {

  vkDeviceWaitIdle(m_logical_device);


  resources::shader_registry->clear();
  resources::mesh_registry->clear();

	vkDestroySemaphore(m_logical_device, m_render_finished_semaphore, nullptr);
	vkDestroySemaphore(m_logical_device, m_image_available_semaphore, nullptr);

  clean_up_swap_chain();

  vkDestroyDescriptorPool(m_logical_device, m_descriptor_pool, nullptr);
  vkDestroyDescriptorSetLayout(m_logical_device, m_descriptor_set_layout, nullptr);
  vmaUnmapMemory(m_vkallocator, m_uniform_allocation);
  destroy_buffer(m_uniform_buffer, m_uniform_allocation);
  vkDestroyCommandPool(m_logical_device, m_command_pool, nullptr);

  vmaDestroyAllocator(m_vkallocator);

  vkDestroyDevice(m_logical_device, 
                  nullptr);

  vkDestroySurfaceKHR(m_vulkan_instance, 
                      m_window_surface, 
                      nullptr);

  if (enable_validation_layers) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(m_vulkan_instance, "vkDestroyDebugReportCallbackEXT");
    func(m_vulkan_instance, m_report_callback, nullptr);
  }
  vkDestroyInstance(m_vulkan_instance, nullptr);
  for(const char * str : m_validation_layers) {
    delete [] str;
  }
}


void begin_frame() {
  m_scene.clear();
}

void end_frame() {

	uint32_t image_index;
	VkResult result = vkAcquireNextImageKHR(m_logical_device, 
                                          m_swap_chain, 
                                          std::numeric_limits<uint64_t>::max(), 
                                          m_image_available_semaphore, 
                                          VK_NULL_HANDLE, 
                                          &image_index);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreate_swap_chain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  build_command_buffer(image_index);

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore wait_semaphores[] = {m_image_available_semaphore};
	VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphores;
	submit_info.pWaitDstStageMask = wait_stages;

  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &m_command_buffers[image_index];

  VkSemaphore signal_semaphores[] = {m_render_finished_semaphore};
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  if (vkQueueSubmit(m_graphics_queue, 
                    1, 
                    &submit_info, 
                    VK_NULL_HANDLE) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signal_semaphores;

	VkSwapchainKHR swapChains[] = {m_swap_chain};
	present_info.swapchainCount = 1;
	present_info.pSwapchains = swapChains;
	present_info.pImageIndices = &image_index;
	present_info.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(m_present_queue, 
                             &present_info);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    recreate_swap_chain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  vkQueueWaitIdle(m_present_queue);
}

void render_mesh(MeshData* mesh_data, 
                 const glm::mat4& model_mat ) {

  m_scene.add_mesh(static_cast<VkMeshData*>(mesh_data),
                   model_mat);

}

void set_camera(const glm::mat4& camera_mat) {
  m_scene.set_camera(camera_mat);
}

}
}
