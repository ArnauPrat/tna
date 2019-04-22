
#ifndef _TNA_TLOG_H_
#define _TNA_TLOG_H_

#include <fstream>

namespace tna 
{
class TnaLog;
extern TnaLog* p_log;

class TnaLog {
public:
  TnaLog(const char* log_filename);
  ~TnaLog();

  void log( const char* message, ... );
  void error( const char* message, ... );
  void warning( const char* message, ... );

private:
  void Message(const char* type, const char* message );

  std::ofstream   m_log_file;
  int32_t         m_errors;
};

}

#endif
