
#ifndef _TNA_BUFFER_H_
#define _TNA_BUFFER_H_ 

namespace tna
{

#define _TNA_RENDERING_INVALID_BUFFER nullptr

using vertex_buffer_t = void*;
using index_buffer_t = void*;


vertex_buffer_t*
create_vertex_buffer(void* data,
                     size_t size);

void
destroy_vertex_buffer(vertex_buffer_t* buffer);

index_buffer_t*
create_index_buffer(void* data,
                    size_t size);

void
destroy_index_buffer(index_buffer_t* buffer);

} /* tna */ 

#endif /* ifndef _TNA_BUFFER_H_ */
