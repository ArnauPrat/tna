

#ifndef _TNA_SHADER_H_
#define _TNA_SHADER_H_ value

#include <string>

namespace tna 
{
struct Shader 
{

  static Shader* 
  load(const std::string& path);

  static void 
  unload(Shader* shader);

};

} /* tna */ 
#endif /* ifndef _TNA_SHADER_H_ */
