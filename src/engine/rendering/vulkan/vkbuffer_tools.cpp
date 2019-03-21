

#include "../../common.h"
#include "vkbuffer_tools.h"

#include <string.h>

namespace tna 
{

/*static uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(m_physical_device, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");

}*/

void 
create_buffer(VmaAllocator allocator,
              VkDeviceSize size, 
              VkBufferUsageFlags buffer_usage, 
              VmaMemoryUsage memory_usage,
              VkBuffer* buffer, 
              VmaAllocation* buffer_allocation) 
{
  VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  bufferInfo.size = size;
  bufferInfo.usage = buffer_usage;

  VmaAllocationCreateInfo alloc_info = {};
  alloc_info.usage = memory_usage;

  if(vmaCreateBuffer(allocator, 
                     &bufferInfo, 
                     &alloc_info, 
                     buffer, 
                     buffer_allocation, 
                     NULL) != VK_SUCCESS)  {
    log->error("Vulkan: failed to allocate memory buffer!");
    report_error(TNA_ERROR::E_RENDERER_RESOURCE_ALLOCATION_ERROR);
  }
  return; 
}

void 
destroy_buffer(VmaAllocator allocator,
               VkBuffer buffer,
               VmaAllocation buffer_allocation) 
{
  vmaDestroyBuffer(allocator,
                   buffer,
                   buffer_allocation);

  return;
}

void
copy_buffer(VkDevice device,
            VkQueue queue,
            VkCommandPool command_pool,
            VmaAllocator allocator,
            VkBuffer srcBuffer, 
            VkBuffer dstBuffer, 
            VkDeviceSize size) 
{
  VkCommandBufferAllocateInfo alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = command_pool;
  alloc_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(device, &alloc_info, &command_buffer);

  VkCommandBufferBeginInfo begin_info = {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(command_buffer, &begin_info);

  VkBufferCopy copyRegion = {};
  copyRegion.srcOffset = 0; // Optional
  copyRegion.dstOffset = 0; // Optional
  copyRegion.size = size;
  vkCmdCopyBuffer(command_buffer, srcBuffer, dstBuffer, 1, &copyRegion);

  vkEndCommandBuffer(command_buffer);

  VkSubmitInfo submit_info = {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffer;

  vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
  vkQueueWaitIdle(queue);

  vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);

  return;
}

void 
create_vertex_buffer(VkDevice device,
                     VkQueue queue,
                     VkCommandPool pool,
                     VmaAllocator allocator,
                     const Vertex* vertices, 
                     uint32_t num_vertices,
                     VkBuffer* buffer, 
                     VmaAllocation* buffer_memory) 
{

  VkDeviceSize buffer_size = sizeof(Vertex) * num_vertices;

  VkBuffer staging_buffer;
  VmaAllocation staging_buffer_allocation;

  create_buffer(allocator,
                buffer_size, 
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                VMA_MEMORY_USAGE_CPU_ONLY, 
                &staging_buffer, 
                &staging_buffer_allocation);

  void* data;
  vmaMapMemory(allocator, staging_buffer_allocation, &data);
  memcpy(data, vertices, (size_t) buffer_size);
  vmaUnmapMemory(allocator, staging_buffer_allocation);

  create_buffer(allocator,
                buffer_size, 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                VMA_MEMORY_USAGE_GPU_ONLY,
                buffer, 
                buffer_memory);

  copy_buffer(device,
              queue,
              pool,
              allocator,
              staging_buffer, *buffer, buffer_size);

  destroy_buffer(allocator,
                 staging_buffer,
                 staging_buffer_allocation);

  return;
}

void 
create_index_buffer(VkDevice device,
                    VkQueue queue,
                    VkCommandPool pool,
                    VmaAllocator allocator,
                    const uint32_t* indices, 
                    uint32_t num_indices,
                    VkBuffer* buffer, 
                    VmaAllocation* buffer_memory) {

  VkDeviceSize buffer_size = sizeof(uint32_t) * num_indices;

  VkBuffer staging_buffer;
  VmaAllocation staging_buffer_allocation;

  create_buffer(allocator,
                buffer_size, 
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                VMA_MEMORY_USAGE_CPU_ONLY, 
                &staging_buffer, 
                &staging_buffer_allocation);

  void* data;
  vmaMapMemory(allocator, staging_buffer_allocation, &data);
  memcpy(data, indices, (size_t) buffer_size);
  vmaUnmapMemory(allocator, staging_buffer_allocation);

  create_buffer(allocator,
                buffer_size, 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                VMA_MEMORY_USAGE_GPU_ONLY,
                buffer, 
                buffer_memory);

  copy_buffer(device, queue, pool, allocator,staging_buffer, *buffer, buffer_size);

  destroy_buffer(allocator,
                 staging_buffer,
                 staging_buffer_allocation);

  return;
}

} /* tna */ 
