

#ifndef _TNA_COMMON_H_
#define _TNA_COMMON_H_ value

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "log.h"
#include "types.h"
#include "error.h"

#include <furious/common/dyn_array.h>

namespace tna
{

template<typename T>
using DynArray = furious::DynArray<T>;
  
} /* tna */ 


#endif /* ifndef _TNA_COMMON_H_ */
