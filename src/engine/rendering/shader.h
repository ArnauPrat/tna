

#ifndef _TNA_SHADER_H_
#define _TNA_SHADER_H_ value

#include <string>

namespace tna 
{

struct TnaShader 
{

  void* p_data;

  static TnaShader* 
  load(const std::string& path);

  static void 
  unload(TnaShader* shader);

};

} /* tna */ 
#endif /* ifndef _TNA_SHADER_H_ */
