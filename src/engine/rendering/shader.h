

#ifndef _TNA_SHADER_H_
#define _TNA_SHADER_H_ value

#include <string>

namespace tna 
{

using shader_t = void*;

shader_t* 
shader_create(const char* path);

void 
shader_destroy(shader_t* shader);

} /* tna */ 
#endif /* ifndef _TNA_SHADER_H_ */
