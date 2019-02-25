

#ifndef _TNA_VKDEVICE_H_
#define _TNA_VKDEVICE_H_

#include <vulkan/vulkan.h>
#include <vector>

namespace tna
{

/**
 * \brief Structure used to store the indices to the different families of
 * available queues
 */
struct QueueFamilyIndices 
{
	int32_t m_graphics_queue = -1;
  int32_t m_present_queue = -1;

  void reset()
  {
    m_graphics_queue = -1;
    m_present_queue = -1;
  }

  bool is_complete() const 
  {
    return  m_graphics_queue != -1 &&
            m_present_queue != -1;
  }
};


/**
 * \brief Checks the support of the physical device for the desired extensions
 *
 * \param device The physical device to check the extensions support of
 * \param desired_extensions The extensions wanted to check
 *
 * \return Returns true if all the desired extensions are supported
 */
bool 
check_device_extension_support(VkPhysicalDevice device,
                               const char** device_extensions,
                               uint32_t num_device_extensions);


/**
 * \brief Checks if the given physical device is suitable for our application
 *
 * \param device The physical device to check for
 * \param surface The rendering surface 
 *
 * \return Returns true if the device is suitable
 */
bool 
is_device_suitable(VkPhysicalDevice device, 
                   VkSurfaceKHR surface,
                   const QueueFamilyIndices* queue_indices,
                   const char** device_extensions,
                   uint32_t num_device_extensions); 
  
} /* tna
 */ 

#endif
