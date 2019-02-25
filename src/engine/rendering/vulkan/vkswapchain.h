

#ifndef _TNA_VKSWAPCHAIN_H_
#define _TNA_VKSWAPCHAIN_H_

#include <vulkan/vulkan.h>

namespace tna 
{

/**
 * \brief Struct used to store the Swap Chain capabilities and support details
 */
struct SwapChainSupportDetails 
{
  SwapChainSupportDetails();
  ~SwapChainSupportDetails();

  VkSurfaceCapabilitiesKHR        m_capabilities;
  uint32_t                        m_num_formats;
  uint32_t                        m_num_present_modes;
  VkSurfaceFormatKHR*             m_formats;
  VkPresentModeKHR*               m_present_modes;
};

/**
 * \brief Queries the physical device for the actual swap chain support
 *
 * \param device The physical device to query the support from
 * \param surface The rendering surface
 *
 * \return Returns a SwapChainSupportDetails structure with the given available
 * support
 */
void 
query_sc_support_details(VkPhysicalDevice device, 
                         VkSurfaceKHR surface,
                         SwapChainSupportDetails* support_details);

/**
 * \brief Given the different available surface formats for the swap chain,
 * choses the best one
 *
 * \param formats A vector with the available surface formats
 * \param num_formats The number of formats
 *
 * \return Returns the chosen surface format
 */
VkSurfaceFormatKHR 
choose_sc_surface_format(const VkSurfaceFormatKHR* formats, 
                         uint32_t num_formats);

/**
 * \brief Given the different available present modes, choses the best one
 *
 * \param present_modes A vector with the available present modes
 * \param num_modes The number of modes
 *
 * \return The choosen present mode
 */
VkPresentModeKHR 
choose_sc_present_mode(const VkPresentModeKHR* modes,
                       uint32_t num_modes);

/**
 * \brief Chooses the extent of the swap chain
 *
 * \param capabilities The capabilities of the surface
 * \param width The width of the viewport 
 * \param height The height of the viewport 
 *
 * \return Returns the choosen extent
 */
VkExtent2D 
choose_sc_extent(const VkSurfaceCapabilitiesKHR& capabilities, 
                 uint32_t width,
                 uint32_t height);
  
} /* tna */ 

#endif
