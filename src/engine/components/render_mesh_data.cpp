

#include "render_mesh_data.h"


namespace tna 
{

void
render_mesh_data_init(render_mesh_data_t* render_mesh,
                 const char* mesh)
{
  render_mesh->m_handler = rendering_scene_create_object(p_rendering_scene, 
                                                         render_object_type_t::E_MESH);
  rendering_scene_set_mesh(p_rendering_scene,
                           render_mesh->m_handler, 
                           mesh);
}

void
render_mesh_data_release(render_mesh_data_t* render_mesh)
{
  rendering_scene_destroy_object(p_rendering_scene, 
                                 render_mesh->m_handler);
}
  
} /* render_mesh_data */ 
