

#include "../config.h"
#include "../common.h"
#include "rendering.h"
#include <cstring>
#include <set>
#include <limits>

namespace tna {

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

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR        m_capabilities;
  std::vector<VkSurfaceFormatKHR> m_formats;
  std::vector<VkPresentModeKHR>   m_present_modes;
};

std::vector<const char*>  layers;
const std::vector<const char*> device_extensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};


VkInstance                vulkan_instance;
VkSurfaceKHR              window_surface;
VkDebugReportCallbackEXT  report_callback;
VkPhysicalDevice          physical_device = VK_NULL_HANDLE;
VkDevice                  logical_device = VK_NULL_HANDLE;
VkSwapchainKHR            swap_chain = VK_NULL_HANDLE;

QueueFamilyIndices 			  queue_indices;
VkQueue                   graphics_queue;
VkQueue                   present_queue;

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

SwapChainSupportDetails   query_sc_support_details(VkPhysicalDevice device, VkSurfaceKHR surface) {
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
                                         &format_count, sc_support_details.m_formats.data());
  }

  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);
  if (present_mode_count != 0) {
    sc_support_details.m_present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, sc_support_details.m_present_modes.data());
  }
  return sc_support_details;
}

VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) {
  if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED) {
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto& available_format : available_formats) {
    if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return available_format;
    }
  }

  return available_formats[0];
}

VkPresentModeKHR choose_swap_present_format(const std::vector<VkPresentModeKHR> available_present_modes) {
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

VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, const Config& config) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actual_extent = {config.m_viewport_width, config.m_viewport_height};

        actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
        actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actual_extent.height));

        return actual_extent;
    }
}



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


bool check_device_extension_support(VkPhysicalDevice device) {
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

    for (const auto& extension : available_extensions) {
      log->log("Found Vulkan device extension %s", extension.extensionName);
      required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  bool extensions_supported = check_device_extension_support(device);

  bool swap_chain_adequate = false;
  if(extensions_supported) {
    SwapChainSupportDetails sc_support_details = query_sc_support_details(device, surface);
    swap_chain_adequate = !sc_support_details.m_formats.empty() && !sc_support_details.m_present_modes.empty();
  }

  return queue_indices.is_complete() && extensions_supported && swap_chain_adequate;
  }

void init_rendering(const Config& config, GLFWwindow* window) {


  // Getting Available Validation Layers
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()); 

  for (const std::string& layer : config.m_vk_validation_layers) {
    bool layer_found = false;
    for (const auto& layerProperties : available_layers) {
      if (strcmp(layer.c_str(), layerProperties.layerName) == 0) {
        log->log("Found validation layer %s", layer.c_str());
        layer_found = true;
        break;
      }
    }

    if (!layer_found) {
      throw std::runtime_error("Validation layer "+layer+" not found");
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
    create_info.enabledLayerCount = static_cast<uint32_t>(config.m_vk_validation_layers.size());
    for(const std::string& str : config.m_vk_validation_layers) {
      layers.push_back(str.c_str());
    }
    create_info.ppEnabledLayerNames = layers.data();
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
  create_info.enabledLayerCount = 0;


  if(vkCreateInstance(&create_info, 
                      nullptr, 
                      &vulkan_instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }

  if (glfwCreateWindowSurface(vulkan_instance, 
                              window, 
                              nullptr, 
                              &window_surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }

  // Adding debug callback
  if(enable_validation_layers) {
    VkDebugReportCallbackCreateInfoEXT debug_callback_info = {};
    debug_callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    debug_callback_info.pfnCallback = debug_callback;

    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(vulkan_instance, "vkCreateDebugReportCallbackEXT");
    func(vulkan_instance, &debug_callback_info, nullptr, &report_callback);
  }

	// Creating Physical Device
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(vulkan_instance, 
                             &device_count, 
                             nullptr);

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(vulkan_instance, 
                             &device_count, 
                             devices.data());

  for (const auto& device : devices) {
    queue_indices.reset();

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
				queue_indices.m_graphics_queue = i;
			}

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, window_surface, &presentSupport);
      if(presentSupport) {
        queue_indices.m_present_queue = i;
      }

			i++;
		}

    if (is_device_suitable(device, window_surface)) {
      physical_device = device;
      break;
    }
  }


  if (physical_device == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }


  // Creating logical devie
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<int> unique_queue_families = {queue_indices.m_graphics_queue, queue_indices.m_present_queue};
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
  createInfo.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
  createInfo.ppEnabledExtensionNames = device_extensions.data();

  if (enable_validation_layers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    createInfo.ppEnabledLayerNames = layers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physical_device, 
                     &createInfo, 
                     nullptr, 
                     &logical_device) != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }

  // Retrieving queues
  vkGetDeviceQueue(logical_device, queue_indices.m_graphics_queue, 0, &graphics_queue);
  vkGetDeviceQueue(logical_device, queue_indices.m_present_queue, 0, &present_queue);

  // Creating Swap chain
  SwapChainSupportDetails swap_chain_support = query_sc_support_details(physical_device, window_surface);
  VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support.m_formats);
  VkPresentModeKHR present_mode = choose_swap_present_format(swap_chain_support.m_present_modes);
  VkExtent2D extent = choose_swap_extent(swap_chain_support.m_capabilities, config);

  uint32_t image_count = swap_chain_support.m_capabilities.minImageCount + 1;
  if (swap_chain_support.m_capabilities.maxImageCount > 0 && image_count > swap_chain_support.m_capabilities.maxImageCount) {
    image_count = swap_chain_support.m_capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR swap_chain_create_info = {};
  swap_chain_create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swap_chain_create_info.surface          = window_surface;
  swap_chain_create_info.minImageCount    = image_count;
  swap_chain_create_info.imageFormat      = surface_format.format;
  swap_chain_create_info.imageColorSpace  = surface_format.colorSpace;
  swap_chain_create_info.imageExtent      = extent;
  swap_chain_create_info.imageArrayLayers = 1;
  swap_chain_create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  uint32_t queue_family_indices[] = {(uint32_t) queue_indices.m_graphics_queue, 
                                    (uint32_t) queue_indices.m_present_queue};

  if (queue_indices.m_graphics_queue != queue_indices.m_present_queue) {
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

  if (vkCreateSwapchainKHR(logical_device, 
                           &swap_chain_create_info, 
                           nullptr, 
                           &swap_chain) != VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  }
}

void terminate_rendering() {


  vkDestroySwapchainKHR(logical_device, swap_chain, nullptr);
  vkDestroyDevice(logical_device, nullptr);
  vkDestroySurfaceKHR(vulkan_instance, window_surface, nullptr);

  if (enable_validation_layers) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(vulkan_instance, "vkDestroyDebugReportCallbackEXT");
    func(vulkan_instance, report_callback, nullptr);
  }
  vkDestroyInstance(vulkan_instance, nullptr);
}



}
