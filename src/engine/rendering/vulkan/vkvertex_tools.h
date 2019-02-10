

#ifndef _TNA_VKVERTEX_TOOLS_H_
#define _TNA_VKVERTEX_TOOLS_H_ value

#include "vkrenderer.h"

namespace tna 
{

VkVertexInputBindingDescription 
get_binding_description(); 

void
get_attribute_descriptions(VkVertexInputAttributeDescription* descriptors);  

} /* tna */ 
#endif /* ifndef _TNA_VKVERTEX_TOOLS_H_ */
