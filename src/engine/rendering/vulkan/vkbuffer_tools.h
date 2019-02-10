


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
create_buffer(VkDeviceSize size, 
              VkBufferUsageFlags buffer_usage, 
              VmaMemoryUsage mem_usage,
              VkBuffer* buffer, 
              VmaAllocation* buffer_allocation);

void 
destroy_buffer(VkBuffer buffer,
               VmaAllocation buffer_allocation);

/**
 * \brief Copies the content of one buffer to another
 *
 * \param srcBuffer The source buffer
 * \param dstBuffer The destination buffer
 * \param size The size in bytes to copy
 */
void 
copy_buffer(VkBuffer src_buffer, 
            VkBuffer dst_buffer, 
            VkDeviceSize size);

/**
 * \brief Creates a vertex buffer
 *
 * \param vertices The vertices array
 * \param buffer The created buffer
 * \param buffer_memory The created memory
 */
void
create_vertex_buffer(const Vertex* vertices, 
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
create_index_buffer(const uint32_t* indices, 
                    uint32_t num_indices,
                    VkBuffer* buffer, 
                    VmaAllocation* alloc_info);

  
} /* tna */ 

#endif /* ifndef _TNA_BUFFER_TOOLS_H_ */
