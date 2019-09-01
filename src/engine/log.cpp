#include "log.h"
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

#define TIMED_MESSAGE( log, message ) \
                             time_t rawtime;  \
                             time(&rawtime); \
                             char* str_time = ctime(&rawtime); \
                             fprintf(log->p_log_file, "%s %s\n", str_time, message);

namespace tna 
{

log_t* p_log = nullptr;

void 
log_aux(log_t* log, 
        const char* type, 
        const char* message );

void
log_create(const char* filename)
{
  p_log = new log_t();
  log_init(p_log, filename);
}

void
log_destroy()
{
  if(p_log)
  {
    log_release(p_log);
    delete p_log;
    p_log = nullptr;
  }
}

void
log_init(log_t* log, const char* filename)
{
  log->p_log_file = fopen(filename, "r");
  log_message(log, "Started execution");
}



void
log_release(log_t* log)
{
  log_message(log, "Finished execution");
  if(log->p_log_file) 
  {
    fclose(log->p_log_file);
  }
  if(log->m_errors > 0) 
  {
    printf("Execution finished with errors. Check the log file\n");
  }
}

void log_message(log_t* log, const char* message, ...) {
    assert(log != nullptr && "Logfile is not initialized");
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
    log_aux(log,"LOG", buffer);
}
void log_error(log_t* log, const char* message, ...) {
    assert(log != nullptr && "Logfile is not initialized");
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
    log_aux(log, "ERROR", buffer);
}

void log_warning(log_t* log, const char* message, ...) {
    assert(log != nullptr && "Logfile is not initialized");
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
    log_aux(log, "WARNING", buffer);
}

void log_aux(log_t* log, const char* type, const char* message) {
    assert(log != nullptr && "Logfile is not initialized");
    char buffer[VARIADIC_BUFFER_SIZE];
    snprintf(buffer, VARIADIC_BUFFER_SIZE, ": %s: %s",type, message);
    TIMED_MESSAGE(log, buffer);
}

}

#undef EXTRACT_VARIADIC
#undef VARIADIC_BUFFER_SIZE
