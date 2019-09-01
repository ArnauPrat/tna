


#include "renderer.h"
#include "rendering_scene.h"

namespace tna
{

rendering_scene_t* p_rendering_scene = nullptr;

void
rendering_scene_create()
{
  p_rendering_scene = new rendering_scene_t();
  rendering_scene_init(p_rendering_scene);
}

void
rendering_scene_destroy()
{
  rendering_scene_release(p_rendering_scene);
  delete p_rendering_scene;
}
  
} /* tna */ 
