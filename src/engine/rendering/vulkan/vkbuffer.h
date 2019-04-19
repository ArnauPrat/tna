

#ifndef _TNA_VULKAN_BUFFER_H_
#define _TNA_VULKAN_BUFFER_H_ 

#include "../../common.h"
#include "../buffer.h"
#include <vulkan/vulkan.h>
#include "vkmem_alloc.h"

namespace  tna
{

struct VkVertexBuffer 
{
  VkBuffer        m_buffer;
  VmaAllocation   m_allocation;
  uint32_t        m_num_vertices;
};

struct VkIndexBuffer 
{
  VkBuffer        m_buffer;
  VmaAllocation   m_allocation;
  uint32_t        m_num_indices;
};

  
} /*  tna */ 


#endif /* ifndef _TNA_VULKAN_BUFFER_H_ */
