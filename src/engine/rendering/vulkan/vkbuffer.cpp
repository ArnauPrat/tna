
#include "vkbuffer_tools.h"
#include "vkrenderer.h"
#include "vkbuffer.h"

namespace tna
{
  
TnaVertexBuffer
create_vertex_buffer(void* data,
                     size_t size)
{
  VkVertexBuffer* vertex_buffer = new VkVertexBuffer();
  create_vertex_buffer(p_renderer->m_logical_device, 
                       p_renderer->m_graphics_queue,
                       p_renderer->m_command_pool,
                       p_renderer->m_vkallocator,
                       data, 
                       size,
                       &vertex_buffer->m_buffer,
                       &vertex_buffer->m_allocation);
  return TnaVertexBuffer{vertex_buffer};
}

void
destroy_vertex_buffer(TnaVertexBuffer* buffer)
{
  VkVertexBuffer* vertex_buffer = (VkVertexBuffer*)(buffer->p_data);
  vmaDestroyBuffer(p_renderer->m_vkallocator,
                   vertex_buffer->m_buffer,
                   vertex_buffer->m_allocation);
  delete vertex_buffer;
}

TnaIndexBuffer
create_index_buffer(void* data,
                    size_t size)
{
  VkIndexBuffer* index_buffer = new VkIndexBuffer();
  create_index_buffer(p_renderer->m_logical_device,
                      p_renderer->m_graphics_queue,
                      p_renderer->m_command_pool,
                      p_renderer->m_vkallocator,
                      data, 
                      size,
                      &index_buffer->m_buffer,
                      &index_buffer->m_allocation);
  return TnaIndexBuffer{index_buffer};

}

void
destroy_index_buffer(TnaIndexBuffer* buffer)
{
  VkIndexBuffer* index_buffer = (VkIndexBuffer*)(buffer->p_data);
  vmaDestroyBuffer(p_renderer->m_vkallocator,
                   index_buffer->m_buffer,
                   index_buffer->m_allocation);

  delete index_buffer;
}

} /* tna */ 
