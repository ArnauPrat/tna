

#include "log.h"
#include "error.h"

#include "stdlib.h"

namespace tna
{

const char* error_messages[(uint32_t)(TNA_ERROR::E_NUM_ERRORS)] = 
{          
  "Success",
  "IO:File Not Found Error",
  "IO:Unexpected Input Error",
  "RENDERER:Resource Allocation Error",
  "RENDERER:Unexpected Initialization Error",
  "RENDERER:Unexpected Runtime Error",
  "RENDERER:Vulkan API Error",
  "RENDERER:Gui Error",
};                                                                              

void
report_error(TNA_ERROR error_code)
{
  TNA_LOG_ERROR("%s", error_messages[(uint32_t)error_code]);
  log_destroy();
  abort();
}
}
