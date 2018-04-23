


#include "common.h"
#include "config.h"
#include "game_app.h"
#include <fstream>
#include <furious/furious.h>
#include <glm/glm.hpp>


namespace tna {

Log*        log     = nullptr;
GLFWwindow* window  = nullptr;
GameApp*    current_app = nullptr;
VkInstance  vulkan_instance;
Config      config;
std::vector<const char*> layers;

VkDebugReportCallbackEXT callback;

VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

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

bool isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    //return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
    //       deviceFeatures.geometryShader;
    return true;
}


static void key_callback(GLFWwindow* window, 
                         int key, 
                         int scancode, 
                         int action, 
                         int mods)
{
  current_app->on_key_event(window, key, scancode, action, mods);
}

static bool file_exists(const std::string& filename) {
  std::ifstream file(filename);
  return file.good();
}

void initialize() {

  // Initializing loging system
  log = new Log("./logfile.log");

  // Reading engine's config
  if(file_exists("./config.ini")) {
    config = load_config("./config.ini");
  } 

  // Initializing Furious
  furious::init();

  // Creating Window
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(config.m_viewport_width, 
                            config.m_viewport_height, 
                            "TNA", 
                            config.m_fullscreen ? glfwGetPrimaryMonitor() : NULL,
                            NULL);
  if (!window) {
    log->error("Error creating window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);


  // Initializing Vulkan context
  uint32_t extension_count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
  std::vector<VkExtensionProperties> supported_extensions(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, supported_extensions.data());
  for(VkExtensionProperties extension : supported_extensions) {
    log->log("Available Vulkan extension %s", extension.extensionName);
  }

  VkApplicationInfo app_info = {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "TNA";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "TNA Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;
  

  // Getting Available Validation Layers
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()); 

  for (std::string& layer : config.m_vk_validation_layers) {
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


  VkInstanceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  if(config.m_vk_validation_layers.size() > 0) {
    create_info.enabledLayerCount = static_cast<uint32_t>(config.m_vk_validation_layers.size());
    for(std::string& str : config.m_vk_validation_layers) {
      layers.push_back(str.c_str());
    }
    create_info.ppEnabledLayerNames = layers.data();
  }


  std::vector<const char*> extensions;
  unsigned int glfw_extensions_count = 0;
  const char** glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extensions_count);
  for(uint32_t i = 0; i < glfw_extensions_count; ++i) {
    extensions.push_back(glfw_extensions[i]);
  }

  if(enableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();
  create_info.enabledLayerCount = 0;


  if(vkCreateInstance(&create_info, nullptr, &vulkan_instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }

  if(enableValidationLayers) {
    VkDebugReportCallbackCreateInfoEXT debug_callback_info = {};
    debug_callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    debug_callback_info.pfnCallback = debug_callback;

    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(vulkan_instance, "vkCreateDebugReportCallbackEXT");
    func(vulkan_instance, &debug_callback_info, nullptr, &callback);
  }

  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(vulkan_instance, &device_count, nullptr);
  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(vulkan_instance, &device_count, devices.data());

  for (const auto& device : devices) {
    if (isDeviceSuitable(device)) {
      physicalDevice = device;
      break;
    }
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }

}

void terminate() {

  if(current_app != nullptr) {
    current_app->on_app_finish();
  }

  if (enableValidationLayers) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(vulkan_instance, "vkDestroyDebugReportCallbackEXT");
    func(vulkan_instance, callback, nullptr);
  }

  vkDestroyInstance(vulkan_instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
  furious::release();
  delete log;

}

void run(GameApp* game_app) {

  current_app = game_app;
  current_app->on_app_start();

  while (!glfwWindowShouldClose(window))
  {
    // Keep running
    glfwPollEvents();
    current_app->on_frame_update();
    glfwSwapBuffers(window);
  }

  current_app->on_app_finish();
  current_app = nullptr;

}

}
