
#ifndef _TNA_TLOG_H_
#define _TNA_TLOG_H_

#include "types.h"
#include <stdio.h>

namespace tna 
{
struct log_t;
extern log_t*  p_log;

  

struct log_t 
{
  FILE*           p_log_file = nullptr;
  int32_t         m_errors = 0;
};

void
log_create(const char* filename);

void
log_destroy();

void
log_init(log_t* log, 
         const char* filename);

void
log_release(log_t* log);

void 
log_message(log_t* log, 
            const char* message, ... );

void 
log_error(log_t* log, 
          const char* message, ... );

void 
log_warning(log_t* log, 
            const char* message, ... );

#define TNA_LOG_INFO(message, ...) \
              if(p_log)\
              { \
                log_message(p_log, message, ##__VA_ARGS__);\
              }

#define TNA_LOG_ERROR(message, ...)\
              if(p_log)\
              { \
                log_error(p_log, message, ##__VA_ARGS__);\
              }

#define TNA_LOG_WARNING(message, ...)\
              if(p_log)\
              { \
                log_warning(p_log, message, ##__VA_ARGS__);\
              }


}

#endif
