
#include "vkswapchain.h"

#include <inttypes.h>
#include <limits.h>
#include <algorithm>

namespace tna 
{

SwapChainSupportDetails::SwapChainSupportDetails() :
m_num_formats(0),
m_num_present_modes(0),
m_formats(nullptr),
m_present_modes(nullptr)
{
}

SwapChainSupportDetails::~SwapChainSupportDetails()
{
  if(m_formats != nullptr)
  {
    delete [] m_formats;
    m_formats = nullptr;
    m_num_formats = 0;
  }

  if(m_present_modes != nullptr)
  {
    delete [] m_present_modes;
    m_present_modes = nullptr;
    m_num_present_modes = 0;
  }
}

void
query_sc_support_details(VkPhysicalDevice device, 
                         VkSurfaceKHR surface,
                         SwapChainSupportDetails* sc_support_details) 
{
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, 
                                            surface, 
                                            &sc_support_details->m_capabilities);
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, 
                                       surface, 
                                       &sc_support_details->m_num_formats, 
                                       nullptr);
  if (sc_support_details->m_num_formats != 0) 
  {
    sc_support_details->m_formats = new VkSurfaceFormatKHR[sc_support_details->m_num_formats];
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, 
                                         surface, 
                                         &sc_support_details->m_num_formats, 
                                         sc_support_details->m_formats);
  }

  vkGetPhysicalDeviceSurfacePresentModesKHR(device, 
                                            surface, 
                                            &sc_support_details->m_num_present_modes, 
                                            nullptr);
  if (sc_support_details->m_num_present_modes != 0) 
  {
    sc_support_details->m_present_modes = new VkPresentModeKHR[sc_support_details->m_num_present_modes];
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, 
                                              surface, 
                                              &sc_support_details->m_num_present_modes, 
                                              sc_support_details->m_present_modes);
  }
}

VkSurfaceFormatKHR 
choose_sc_surface_format(const VkSurfaceFormatKHR* formats,
                         uint32_t num_formats) 
{
  if (num_formats == 1 && formats[0].format == VK_FORMAT_UNDEFINED) 
  {
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (uint32_t i = 0; i < num_formats; ++i)
  {
    const VkSurfaceFormatKHR* format = &formats[i];
    if (format->format == VK_FORMAT_B8G8R8A8_UNORM && 
        format->colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return *format;
    }
  }

  return formats[0];
}

VkPresentModeKHR 
choose_sc_present_mode(const VkPresentModeKHR* modes,
                       uint32_t num_modes) 
{
    VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (uint32_t i = 0; i < num_modes; ++i) 
    {
      VkPresentModeKHR mode = modes[i];
      if (mode == VK_PRESENT_MODE_MAILBOX_KHR) 
      {
        return mode;
      } 
      else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) 
      {
            best_mode = mode;
      }
    }
    return best_mode;
}

VkExtent2D 
choose_sc_extent(const VkSurfaceCapabilitiesKHR& capabilities, 
                 uint32_t width,
                 uint32_t height) 
{
  if (capabilities.currentExtent.width != UINT_MAX) 
  {
    return capabilities.currentExtent;
  } 
  else 
  {
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

  
} /* tna */ 
