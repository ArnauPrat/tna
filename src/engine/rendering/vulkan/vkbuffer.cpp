
#include "vkbuffer_tools.h"
#include "vkrenderer.h"
#include "vkbuffer.h"

namespace tna
{
  
vertex_buffer_t*
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
  return new vertex_buffer_t(vertex_buffer);
}

void
destroy_vertex_buffer(vertex_buffer_t* buffer)
{
  VkVertexBuffer* vertex_buffer = (VkVertexBuffer*)(*buffer);
  vmaDestroyBuffer(p_renderer->m_vkallocator,
                   vertex_buffer->m_buffer,
                   vertex_buffer->m_allocation);
  delete vertex_buffer;
  delete buffer;
}

index_buffer_t*
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
  return new index_buffer_t(index_buffer);

}

void
destroy_index_buffer(index_buffer_t* buffer)
{
  VkIndexBuffer* index_buffer = (VkIndexBuffer*)(*buffer);
  vmaDestroyBuffer(p_renderer->m_vkallocator,
                   index_buffer->m_buffer,
                   index_buffer->m_allocation);

  delete index_buffer;
  delete buffer;

}

} /* tna */ 
