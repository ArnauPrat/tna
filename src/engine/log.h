
#ifndef _TNA_TLOG_H_
#define _TNA_TLOG_H_

#include <fstream>
#include <sstream>

namespace tna {
class Log {
public:
  Log(const char* log_filename);
  ~Log();

  void print( const char* message, ... );
  void error( const char* message, ... );
  void warning( const char* message, ... );

private:
  void Message(const char* type, const char* message );


  std::ofstream   m_log_file;
};

}

#endif
