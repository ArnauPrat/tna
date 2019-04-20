



#ifndef _TNA_VKRENDERER_GUI_H_
#define _TNA_VKRENDERER_GUI_H_ value

#include "../../../common.h"

struct GLFWwindow;

namespace tna
{

void
vkgui_init();

void
vkgui_recreate();

void
vkgui_begin_frame();

void
vkgui_build_commands(uint32_t index);

void
vkgui_terminate();
  
} /* tna */ 
#endif /* ifndef _TNA_VKRENDERER_GUI_H_ */
