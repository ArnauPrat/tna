

#ifndef _TNA_RENDERING_SCENE_H_
#define _TNA_RENDERING_SCENE_H_

#include "../math/matrix.h"
#include "../math/vector.h"
#include "../tools/colors.h"

#include "renderer.h"
#include "furious/common/dyn_array.h"

namespace tna 
{

#define _TNA_RENDER_HANDLER_INVALID 0xffffffff

struct mesh_data_t;

enum class render_object_type_t 
{
  E_MESH,
};

struct material_desc_t 
{
  vector3_t   m_color = TNA_COLOR_WHITE;
};

struct render_mesh_uniform_t 
{
  matrix4_t           m_model_matrix;
  material_desc_t     m_material;
};

using render_handler_t = uint32_t;

struct render_header_t 
{
  uint32_t                m_offset              = 0;
  render_object_type_t    m_render_object_type  = render_object_type_t::E_MESH;
  bool                    m_active              = false;
  bool                    m_frustrum_visible    = true;
};

struct rendering_scene_t 
{

  furious::DynArray<render_header_t>          m_headers;
  furious::DynArray<uint32_t>                 m_header_gaps;

  render_mesh_uniform_t*                      m_uniforms = nullptr;
  furious::DynArray<mesh_data_t*>             m_meshes;
  furious::DynArray<uint32_t>                 m_gaps;

  matrix4_t                                   m_view_mat = matrix4_t(1.0f);
  matrix4_t                                   m_proj_mat = matrix4_t(1.0f);
  vector3_t                                   m_clear_color = vector3_t(0.0f, 0.0f, 0.0f);

  //size_t                                      m_uniform_alignment;
};

void
rendering_scene_init(rendering_scene_t* scene);

void
rendering_scene_release(rendering_scene_t* scene);

render_handler_t
rendering_scene_create_object(rendering_scene_t* scene,
                              render_object_type_t o_type);

void
rendering_scene_destroy_object(rendering_scene_t* scene,
                               render_handler_t& handler);


void
rendering_scene_set_mesh(rendering_scene_t* scene, render_handler_t handler, 
                         const char* mesh);

void
rendering_scene_set_material(rendering_scene_t* scene,
                             render_handler_t handler, 
                             const material_desc_t& mat_desc);

void
rendering_scene_get_material(rendering_scene_t* scene,
                             render_handler_t handler,
                             material_desc_t* mat_desc);

void
rendering_scene_set_model_mat(rendering_scene_t* scene,
                              render_handler_t handler, 
                              const matrix4_t& mat);

void
rendering_scene_set_frustrum_culling(rendering_scene_t* scene,
                                     render_handler_t handler,
                                     const matrix4_t& proj_matrix);

void 
rendering_scene_set_view_matrix(rendering_scene_t* scene,
                                const matrix4_t& view_mat);

void 
rendering_scene_set_proj_matrix(rendering_scene_t* scene,
                                const matrix4_t& projection_mat);

void
rendering_scene_set_clear_color(rendering_scene_t* scene,
                                const vector3_t& color);

} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
