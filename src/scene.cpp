


#include "scene.h"
#include "engine/game_state.h"
#include "engine/components/render_mesh_data.h"
#include "engine/components/transform.h"
#include "engine/components/transform_matrix.h"
#include "engine/tools/references.h"
#include "engine/resources/resources.h"
#include "engine/tools/colors.h"
#include "components/rotation_speed.h"

namespace tna
{

void
create_terrain(TnaGameState* state)
{
  uint32_t width = 18;
  uint32_t height = 12;
  float TILE_HALF_EDGE = 5;
  for(uint32_t i = 0; i < width; ++i)
  {
    for(uint32_t j = 0; j < height; ++j)
    {
      TnaEntity terrain = create_entity(state);

      TnaTransform* transform = FURIOUS_GET_COMPONENT(terrain, TnaTransform);
      transform->m_scale.x = TILE_HALF_EDGE;
      transform->m_scale.y = 5.0f;
      transform->m_scale.z = TILE_HALF_EDGE;

      transform->m_position.x  = i*TILE_HALF_EDGE*2.0 - TILE_HALF_EDGE*width;
      transform->m_position.y -= 5.0f;
      transform->m_position.z  = j*TILE_HALF_EDGE*2.0 - TILE_HALF_EDGE*height;
      transform->m_dirty = true;

      FURIOUS_ADD_COMPONENT(terrain, 
                            TnaRenderMeshData,
                            "models/cube.obj");

      TnaRenderMeshData* mesh_data = FURIOUS_GET_COMPONENT(terrain, TnaRenderMeshData);
      TnaMaterialDescriptor mat_desc;
      p_rendering_scene->get_material(mesh_data->m_handler, &mat_desc);
      mat_desc.m_color = TNA_COLOR_LIGHT_GREY_2;
      p_rendering_scene->set_material(mesh_data->m_handler, mat_desc);
    }
  }
}

void
create_buildings(TnaGameState* state)
{
}

void
create_cars(TnaGameState* state)
{

  TnaEntity entity1 = create_entity(state);
  FURIOUS_ADD_COMPONENT(entity1, 
                        TnaRenderMeshData,
                        "models/cube.obj");

  TnaRenderMeshData* mesh_data = FURIOUS_GET_COMPONENT(entity1, TnaRenderMeshData);
  TnaMaterialDescriptor mat_desc;
  p_rendering_scene->get_material(mesh_data->m_handler, &mat_desc);
  mat_desc.m_color = TNA_COLOR_RED;
  p_rendering_scene->set_material(mesh_data->m_handler, mat_desc);

  TnaTransform* transform = FURIOUS_GET_COMPONENT(entity1, TnaTransform);
  transform->m_position = {0.0f,50.0f,0.0f};
  transform->m_dirty = true;

  srand(time(NULL));

  
  double factor = 3.1416f / 180.0f;
  for(uint32_t i = 0; i < 20000; ++i)
  {
    TnaEntity entity2 = create_entity(state);
    FURIOUS_ADD_COMPONENT(entity2, 
                          TnaRenderMeshData,
                          "models/cube.obj");

    TnaRenderMeshData* mesh_data = FURIOUS_GET_COMPONENT(entity2, TnaRenderMeshData);

    float speed_factor = rand() / (float)INT_MAX;
    FURIOUS_ADD_COMPONENT(entity2, RotationSpeed, radians(speed_factor*360));
    int seed = rand() % 360;
    float dist = (rand() / (float)INT_MAX)*250.0 + 5.0f;
    float posx = sin(seed*factor)*dist;
    float posz = cos(seed*factor)*dist;
    TnaTransform* transform = FURIOUS_GET_COMPONENT(entity2, TnaTransform);
    transform->m_position = {posx,0.0f,posz};
    transform->m_scale = {0.25f,0.25f,0.25f};
    transform->m_dirty = true;

    TnaMaterialDescriptor mat_desc;
    p_rendering_scene->get_material(mesh_data->m_handler, &mat_desc);
    mat_desc.m_color = TNA_COLOR_BLACK;
    p_rendering_scene->set_material(mesh_data->m_handler, mat_desc);

    FURIOUS_ADD_REFERENCE(entity2, 
                          TNA_REF_PARENT, 
                          entity1);
  }
}

void
create_player(TnaGameState* state)
{
}
  
} /* tna */ 
