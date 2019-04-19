
#ifndef _TNA_BUFFER_H_
#define _TNA_BUFFER_H_ 

namespace tna
{

struct VertexBuffer
{
  void* p_data;
};

struct IndexBuffer
{
  void* p_data;
};

VertexBuffer
create_vertex_buffer(void* data,
                     size_t size);

void
destroy_vertex_buffer(VertexBuffer* buffer);

IndexBuffer
create_index_buffer(void* data,
                    size_t size);

void
destroy_index_buffer(IndexBuffer* buffer);

} /* tna */ 

#endif /* ifndef _TNA_BUFFER_H_ */
