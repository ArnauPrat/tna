


#include "inttypes.h"
#include "vkdevice.h"
#include "string.h"
#include <set>
#include "../../common.h"
#include "vkswapchain.h"

namespace tna
{

bool 
check_device_extension_support(VkPhysicalDevice device,
                               const char** device_extensions,
                               uint32_t num_device_extensions) 
{

    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    VkExtensionProperties* available_extensions = new VkExtensionProperties[extension_count];

    vkEnumerateDeviceExtensionProperties(device, 
                                         nullptr, 
                                         &extension_count, 
                                         available_extensions);

    TNA_LOG_INFO("Checking physical device");
    bool all_found = true;
    for (uint32_t i = 0; i < num_device_extensions; ++i)
    {
      bool extension_found = false;
      const char* next_extension = device_extensions[i];
      TNA_LOG_INFO("Looking for Vulkan device extension %s", next_extension);
      for (uint32_t j = 0; j < extension_count; ++j)
      {
        const char* available_extension = available_extensions[j].extensionName;
        if(strcmp(next_extension, available_extension) == 0)
        {
          TNA_LOG_INFO("Found Vulkan device extension %s", available_extension);
          extension_found = true;
        }
      }
      all_found = all_found && extension_found;
    }

    delete [] available_extensions;

    return all_found;
}

bool 
is_device_suitable(VkPhysicalDevice device, 
                   VkSurfaceKHR surface,
                   const QueueFamilyIndices* queue_indices,
                   const char** device_extensions,
                   uint32_t num_device_extensions) 
{
  bool extensions_supported = check_device_extension_support(device, 
                                                             device_extensions,
                                                             num_device_extensions);

  bool swap_chain_adequate = false;
  if(extensions_supported) 
  {
    SwapChainSupportDetails sc_support_details;
    query_sc_support_details(device, surface, &sc_support_details);
    swap_chain_adequate = sc_support_details.m_num_formats > 0 && sc_support_details.m_num_present_modes > 0;
  }

  return queue_indices->is_complete() && extensions_supported && swap_chain_adequate;
}


  
} 
