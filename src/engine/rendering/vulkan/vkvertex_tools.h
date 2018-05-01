

#ifndef _TNA_VKVERTEX_TOOLS_H_
#define _TNA_VKVERTEX_TOOLS_H_ value

#include "vkrenderer.h"
#include <array>

namespace tna {
namespace rendering {

VkVertexInputBindingDescription get_binding_description(); 

std::array<VkVertexInputAttributeDescription, 3> get_attribute_descriptions();  
} /* rendering */ 
} /* tna */ 
#endif /* ifndef _TNA_VKVERTEX_TOOLS_H_ */
