
#ifndef _TNA_TLOG_H_
#define _TNA_TLOG_H_

#include "types.h"
#include <stdio.h>

namespace tna 
{

void
log_init(const char* filename);

void
log_release();

void 
log_message(const char* message, ... );

void 
log_error(const char* message, ... );

void 
log_warning(const char* message, ... );

#define TNA_LOG_INFO(message, ...) \
                log_message(message, ##__VA_ARGS__);\

#define TNA_LOG_ERROR(message, ...)\
                log_error(message, ##__VA_ARGS__);\

#define TNA_LOG_WARNING(message, ...)\
                log_warning(message, ##__VA_ARGS__);\


}

#endif
