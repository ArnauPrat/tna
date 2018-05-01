

#ifndef _TNA_SHADER_H_
#define _TNA_SHADER_H_ value

#include <vulkan/vulkan.h>
#include <string>

namespace tna {
namespace rendering {

struct Shader {

  static Shader* load(const std::string& path);

  static void unload(Shader* shader);

};

}
} /* tna */ 
#endif /* ifndef _TNA_SHADER_H_ */
