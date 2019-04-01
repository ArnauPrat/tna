


#ifndef _TNA_VKBUFFER_TOOLS_H_
#define _TNA_VKBUFFER_TOOLS_H_ value

#include "../vertex.h"
#include "../../common.h"
#include "vkrenderer.h"

namespace tna 
{


/**
 * \brief Creates a VKBuffer 
 *
 * \param size The size of the buffer to create
 * \param usage The usage flags of the buffer
 * \param properties The memory flags 
 * \param buffer The created buffer 
 * \param buffer_memory The created memory
 */
void 
create_buffer(VmaAllocator allocator,
              VkDeviceSize size, 
              VkBufferUsageFlags buffer_usage, 
              VmaMemoryUsage mem_usage,
              VkBuffer* buffer, 
              VmaAllocation* buffer_allocation);

void 
destroy_buffer(VmaAllocator allocator,
               VkBuffer buffer,
               VmaAllocation buffer_allocation);

/**
 * \brief Copies the content of one buffer to another
 *
 * \param srcBuffer The source buffer
 * \param dstBuffer The destination buffer
 * \param size The size in bytes to copy
 */
void
copy_buffer(VkDevice device,
            VkQueue queue,
            VkCommandPool command_pool,
            VmaAllocator allocator,
            VkBuffer srcBuffer, 
            VkBuffer dstBuffer, 
            VkDeviceSize size);

/**
 * \brief Creates a vertex buffer
 *
 * \param vertices The vertices array
 * \param buffer The created buffer
 * \param buffer_memory The created memory
 */
void
create_vertex_buffer(VkDevice device,
                     VkQueue queue,
                     VkCommandPool pool,
                     VmaAllocator allocator,
                     const Vertex* vertices, 
                     uint32_t num_vertices,
                     VkBuffer* buffer, 
                     VmaAllocation* alloc_info);


/**
 * \brief Creates an index buffer
 *
 * \param vertices The indices array
 * \param buffer The created buffer
 * \param buffer_memory The created memory
 */
void 
create_index_buffer(VkDevice device,
                    VkQueue queue,
                    VkCommandPool pool,
                    VmaAllocator allocator,
                    const uint32_t* indices, 
                    uint32_t num_indices,
                    VkBuffer* buffer, 
                    VmaAllocation* alloc_info);

/**
 * \brief Creates an image
 *
 * \param allocator The allocator to use
 * \param memory_usage The type of memory
 * \param width The width of the image
 * \param height The height of the image
 * \param format The format of the image
 * \param tiling The tiling arrangement
 * \param usage The image usage flags
 * \param image The pointer to the image object to store the image
 * \param image_allocation The pointer to the image allocation info
 */
void
create_image(VmaAllocator allocator,
             VmaMemoryUsage memory_usage,
             uint32_t width, 
             uint32_t height, 
             VkFormat format, 
             VkImageTiling tiling, 
             VkImageUsageFlags usage, 
             VkImage* image, 
             VmaAllocation* image_allocation);
  
/**
 * \brief Destroys an image
 *
 * \param allocator The memory allocator
 * \param image The image to detroy
 * \param allocation The memory allocation
 */
void
destroy_image(VmaAllocator allocator,
              VkImage image,
              VmaAllocation allocation);
} /* tna */ 

#endif /* ifndef _TNA_BUFFER_TOOLS_H_ */
