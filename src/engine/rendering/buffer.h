
#ifndef _TNA_BUFFER_H_
#define _TNA_BUFFER_H_ 

namespace tna
{

struct TnaVertexBuffer
{
  void* p_data;
};

struct TnaIndexBuffer
{
  void* p_data;
};

TnaVertexBuffer
create_vertex_buffer(void* data,
                     size_t size);

void
destroy_vertex_buffer(TnaVertexBuffer* buffer);

TnaIndexBuffer
create_index_buffer(void* data,
                    size_t size);

void
destroy_index_buffer(TnaIndexBuffer* buffer);

} /* tna */ 

#endif /* ifndef _TNA_BUFFER_H_ */
