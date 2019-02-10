


#ifndef _TNA_MAKE_KEYWORD_ENUM

#ifndef _TNA_ERROR_H_
#define _TNA_ERROR_H_ value

#include "types.h"

namespace tna
{

#define _TNA_MAKE_KEYWORD_ENUM                                                          
#define _TNA_ERROR_KEYWORD(symbol, text) symbol 
  enum class TNA_ERROR
  {
#endif
    _TNA_ERROR_KEYWORD(E_SUCCESS = 0, "Success"),
    _TNA_ERROR_KEYWORD(E_IO_FILE_NOT_FOUND, "IO:File Not Found Error"),
    _TNA_ERROR_KEYWORD(E_IO_UNEXPECTED_INPUT_ERROR, "IO:Unexpected Input Error"),

    _TNA_ERROR_KEYWORD(E_RENDERER_RESOURCE_ALLOCATION_ERROR, "RENDERER:Resource Allocation Error"),
    _TNA_ERROR_KEYWORD(E_RENDERER_INITIALIZATION_ERROR, "RENDERER:Unexpected Initialization Error"),
    _TNA_ERROR_KEYWORD(E_RENDERER_RUNTIME_ERROR,"RENDERER:Unexpected Runtime Error"),
    _TNA_ERROR_KEYWORD(E_RENDERER_VULKAN_ERROR,"RENDERER:Vulkan API Error"),

#ifdef _TNA_MAKE_KEYWORD_ENUM
    E_NUM_ERRORS
  };

#undef _TNA_MAKE_KEYWORD_ENUM
#undef _TNA_ERROR_KEYWORD

void
report_error(TNA_ERROR error_code);

}
#endif /* ifndef _TNA_ERROR_H_ */
#endif
