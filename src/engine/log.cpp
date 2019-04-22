#include "log.h"
#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#define VARIADIC_BUFFER_SIZE 1024
#define EXTRACT_VARIADIC( buffer, message ) \
                            va_list larg; \
                            va_start(larg, message); \
                            vsnprintf(buffer, VARIADIC_BUFFER_SIZE, message, larg); \
                            va_end(larg);

#define TIMED_MESSAGE( message ) \
                             time_t rawtime;  \
                             time(&rawtime); \
                             std::string str_time(ctime(&rawtime)); \
                             str_time.erase(str_time.size()-1,1); \
                             m_log_file << str_time << " " << message << std::endl;

namespace tna {


TnaLog::TnaLog(const char* filename) : 
  m_log_file{filename},
  m_errors{0}
{
      log("Started execution");
}

TnaLog::~TnaLog() {
  log("Finished execution");
  if(m_log_file) {
    m_log_file.close();
  }
  if(m_errors > 0) {
    printf("Execution finished with errors. Check the log file\n");
  }
}

void TnaLog::log(const char* message, ...) {
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
        Message("LOG", buffer);
}
void TnaLog::error(const char* message, ...) {
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
      Message("ERROR", buffer);
}

void TnaLog::warning(const char* message, ...) {
    char buffer[VARIADIC_BUFFER_SIZE]; 
    EXTRACT_VARIADIC(buffer, message)
      Message("WARNING", buffer);
}

void TnaLog::Message(const char* type, const char* message) {
    char buffer[VARIADIC_BUFFER_SIZE];
    sprintf(buffer, ": %s: %s",type, message);
    TIMED_MESSAGE(buffer);
}

}

#undef EXTRACT_VARIADIC
#undef VARIADIC_BUFFER_SIZE
