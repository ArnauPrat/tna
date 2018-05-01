


#ifndef _TNA_VKBUFFER_TOOLS_H_
#define _TNA_VKBUFFER_TOOLS_H_ value

#include "../vertex.h"
#include "../../common.h"
#include "vkrenderer.h"
#include <vector>

namespace tna {
namespace rendering {

/**
 * @brief Creates a VKBuffer 
 *
 * @param size The size of the buffer to create
 * @param usage The usage flags of the buffer
 * @param properties The memory flags 
 * @param buffer The created buffer 
 * @param buffer_memory The created memory
 */
void create_buffer( VkDeviceSize size, 
                    VkBufferUsageFlags usage, 
                    VkMemoryPropertyFlags properties, 
                    VkBuffer& buffer, 
                    VkDeviceMemory& buffer_memory);

/**
 * @brief Copies the content of one buffer to another
 *
 * @param srcBuffer The source buffer
 * @param dstBuffer The destination buffer
 * @param size The size in bytes to copy
 */
void copy_buffer( VkBuffer src_buffer, 
                  VkBuffer dst_buffer, 
                  VkDeviceSize size);

/**
 * @brief Creates a vertex buffer
 *
 * @param vertices The vertices array
 * @param buffer The created buffer
 * @param buffer_memory The created memory
 */
void create_vertex_buffer(const std::vector<Vertex>& vertices, 
                          VkBuffer& buffer, 
                          VkDeviceMemory& buffer_memory);

/**
 * @brief Creates an index buffer
 *
 * @param vertices The indices array
 * @param buffer The created buffer
 * @param buffer_memory The created memory
 */
void create_index_buffer(const std::vector<uint32_t>& vertices, 
                          VkBuffer& buffer, 
                          VkDeviceMemory& buffer_memory);

  
} /* rendering */ 
} /* tna */ 

#endif /* ifndef _TNA_BUFFER_TOOLS_H_ */
