


#include "renderer.h"
#include "rendering_scene.h"

namespace tna
{

RenderingScene* p_rendering_scene = nullptr;

void
create_rendering_scene()
{
  p_rendering_scene = new RenderingScene();
}

void
destroy_rendering_scene()
{
  delete p_rendering_scene;
}
  
} /* tna */ 
