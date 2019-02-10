

#include "log.h"
#include "error.h"

#include "stdlib.h"

namespace tna
{

#define _TNA_ERROR_KEYWORD(symbol, text) text

const char* error_messages[(uint32_t)(TNA_ERROR::E_NUM_ERRORS)] = {          
#include "error.h"                                                      
};                                                                              

void
report_error(TNA_ERROR error_code)
{
  log->error("%s", error_messages[(uint32_t)error_code]);
  delete log;
  abort();
}
}
