#include "log.h"
#include "error.h"
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#define VARIADIC_BUFFER_SIZE 1024
#define EXTRACT_VARIADIC( buffer, message ) \
                            va_list larg; \
                            va_start(larg, message); \
                            vsnprintf(buffer, VARIADIC_BUFFER_SIZE, message, larg); \
                            va_end(larg);

#define TIMED_MESSAGE( message ) \
                           time_t rawtime;  \
                           time(&rawtime); \
                           char* str_time = ctime(&rawtime); \
                           fprintf(m_log.p_log_file, "%s %s\n", str_time, message);

namespace tna 
{

struct log_t 
{
  FILE*           p_log_file = nullptr;
  int32_t         m_errors = 0;
};

log_t m_log;

void 
log_aux(const char* type, 
        const char* message );

void
log_init(const char* filename)
{
  m_log.p_log_file = fopen(filename, "w");
  log_message("Started execution");
}



void
log_release()
{
  log_message("Finished execution");
  if(m_log.p_log_file) 
  {
    fclose(m_log.p_log_file);
  }
  if(m_log.m_errors > 0) 
  {
    printf("Execution finished with errors. Check the log file\n");
  }
}

void log_message(const char* message, ...) {
    assert(m_log.p_log_file != nullptr && "Logfile is not initialized");
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
    log_aux("LOG", buffer);
}
void log_error(TNA_ERROR error, const char* message, ...) {
    assert(m_log.p_log_file != nullptr && "Logfile is not initialized");
    log_aux("ERROR", error_messages[(uint32_t)error]);
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
    log_aux("ERROR", buffer);
    log_release();
    abort();
}

void log_warning(const char* message, ...) {
    assert(m_log.p_log_file != nullptr && "Logfile is not initialized");
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
    log_aux("WARNING", buffer);
}

void log_aux(const char* type, const char* message) {
    assert(m_log.p_log_file != nullptr && "Logfile is not initialized");
    char buffer[VARIADIC_BUFFER_SIZE];
    snprintf(buffer, VARIADIC_BUFFER_SIZE, ": %s: %s",type, message);
    TIMED_MESSAGE(buffer);
}

}

#undef EXTRACT_VARIADIC
#undef VARIADIC_BUFFER_SIZE
