

#include "vkbuffer_tools.h"
#include <cstring>
#include <stdexcept>

namespace tna {
namespace rendering {

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

void create_buffer(VkDeviceSize size, 
                   VkBufferUsageFlags buffer_usage, 
                   VmaMemoryUsage memory_usage,
                   VkBuffer& buffer, 
                   VmaAllocation& buffer_allocation) {

  VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  bufferInfo.size = size;
  bufferInfo.usage = buffer_usage;

  VmaAllocationCreateInfo alloc_info = {};
  alloc_info.usage = memory_usage;

  if(vmaCreateBuffer(m_vkallocator, 
                     &bufferInfo, 
                     &alloc_info, 
                     &buffer, 
                     &buffer_allocation, 
                     nullptr) != VK_SUCCESS)  {
    throw std::runtime_error("failed to allocate memory buffer!");
  }

}

void copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

  VkCommandBufferAllocateInfo alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = m_command_pool;
  alloc_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(m_logical_device, &alloc_info, &command_buffer);

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

  vkQueueSubmit(m_graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
  vkQueueWaitIdle(m_graphics_queue);

  vkFreeCommandBuffers(m_logical_device, m_command_pool, 1, &command_buffer);

}

void create_vertex_buffer(const std::vector<Vertex>& vertices, 
                          VkBuffer& buffer, 
                          VmaAllocation& buffer_memory) {

  VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size();

  VkBuffer staging_buffer;
  VmaAllocation staging_buffer_allocation;

  create_buffer(buffer_size, 
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                VMA_MEMORY_USAGE_CPU_ONLY, 
                staging_buffer, 
                staging_buffer_allocation);

  void* data;
  vmaMapMemory(m_vkallocator, staging_buffer_allocation, &data);
  memcpy(data, vertices.data(), (size_t) buffer_size);
  vmaUnmapMemory(m_vkallocator, staging_buffer_allocation);

  create_buffer(buffer_size, 
               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
               VMA_MEMORY_USAGE_GPU_ONLY,
               buffer, 
               buffer_memory);

  copy_buffer(staging_buffer, buffer, buffer_size);

  destroy_buffer(staging_buffer,
                 staging_buffer_allocation);

}

void create_index_buffer(const std::vector<uint32_t>& indices, 
                          VkBuffer& buffer, 
                          VmaAllocation& buffer_memory) {

  VkDeviceSize buffer_size = sizeof(indices[0]) * indices.size();

  VkBuffer staging_buffer;
  VmaAllocation staging_buffer_allocation;

  create_buffer(buffer_size, 
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                VMA_MEMORY_USAGE_CPU_ONLY, 
                staging_buffer, 
                staging_buffer_allocation);

  void* data;
  vmaMapMemory(m_vkallocator, staging_buffer_allocation, &data);
  memcpy(data, indices.data(), (size_t) buffer_size);
  vmaUnmapMemory(m_vkallocator, staging_buffer_allocation);

  create_buffer(buffer_size, 
               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
               VMA_MEMORY_USAGE_GPU_ONLY,
               buffer, 
               buffer_memory);

  copy_buffer(staging_buffer, buffer, buffer_size);

  destroy_buffer(staging_buffer,
                 staging_buffer_allocation);
}

void destroy_buffer(VkBuffer buffer,
                    VmaAllocation buffer_allocation) {

  vmaDestroyBuffer(m_vkallocator,
                   buffer,
                   buffer_allocation);
}

} /* rendering */ 
} /* tna */ 
