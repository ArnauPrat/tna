


#ifndef _TNA_MAKE_KEYWORD_ENUM

#ifndef _TNA_ERROR_H_
#define _TNA_ERROR_H_ value

#include "types.h"

namespace tna
{

  enum class TNA_ERROR
  {
    E_SUCCESS = 0,
    E_IO_FILE_NOT_FOUND, 
    E_IO_UNEXPECTED_INPUT_ERROR, 

    E_RENDERER_RESOURCE_ALLOCATION_ERROR, 
    E_RENDERER_INITIALIZATION_ERROR, 
    E_RENDERER_RUNTIME_ERROR,
    E_RENDERER_VULKAN_ERROR,

    E_RENDERER_GUI_ERROR,

    E_NUM_ERRORS
  };

extern const char* error_messages[(uint32_t)(TNA_ERROR::E_NUM_ERRORS)];

}
#endif /* ifndef _TNA_ERROR_H_ */
#endif
