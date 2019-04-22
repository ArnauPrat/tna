


#include "scene.h"
#include "engine/game_state.h"
#include "engine/components/render_mesh_data.h"
#include "engine/components/transform.h"
#include "engine/components/transform_matrix.h"
#include "engine/resources/resources.h"
#include "engine/tools/colors.h"
#include "components/rotation_speed.h"

namespace tna
{

void
create_terrain(TnaGameState* state)
{
  uint32_t NUM_TILES = 50;
  float TILE_HALF_EDGE = 5.0;
  for(uint32_t i = 0; i < NUM_TILES; ++i)
  {
    for(uint32_t j = 0; j < NUM_TILES; ++j)
    {
      TnaEntity terrain = create_entity(state);

      TnaTransform* transform = FURIOUS_GET_COMPONENT(terrain, TnaTransform);
      transform->m_scale.x = TILE_HALF_EDGE;
      transform->m_scale.y = 5.0f;
      transform->m_scale.z = TILE_HALF_EDGE;

      transform->m_position.x  = i*TILE_HALF_EDGE*2.0 - TILE_HALF_EDGE*NUM_TILES;
      transform->m_position.y -= 5.0f;
      transform->m_position.z  = j*TILE_HALF_EDGE*2.0 - TILE_HALF_EDGE*NUM_TILES;
      transform->m_dirty = false;

      set_static(terrain);

      FURIOUS_ADD_COMPONENT(terrain, TnaRenderMeshData);

      TnaRenderMeshData* render_mesh_data = terrain.get_component<TnaRenderMeshData>();
      render_mesh_data->p_mesh_data = p_mesh_registry->load("models/cube.obj");
      render_mesh_data->m_material.m_color = TNA_COLOR_LIGHT_GREY_2;
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
  FURIOUS_ADD_COMPONENT(entity1, TnaRenderMeshData);
  entity1.get_component<TnaRenderMeshData>()->p_mesh_data = p_mesh_registry->load("models/cube.obj");
  entity1.get_component<TnaTransform>()->m_position = {0.0f,50.0f,0.0f};
  entity1.get_component<TnaRenderMeshData>()->m_material.m_color = {1.0f, 0.0f, 0.0f};

  srand(time(NULL));

  
  double factor = 3.1416f / 180.0f;
  for(uint32_t i = 0; i < 1000; ++i)
  {
    TnaEntity entity2 = create_entity(state);
    FURIOUS_ADD_COMPONENT(entity2, TnaRenderMeshData);
    float speed_factor = rand() / (float)INT_MAX;
    FURIOUS_ADD_COMPONENT(entity2, RotationSpeed, radians(speed_factor*360));
    entity2.get_component<TnaRenderMeshData>()->p_mesh_data = p_mesh_registry->load("models/cube.obj");
    int seed = rand() % 360;
    float dist = (rand() / (float)INT_MAX)*30.0 + 5.0f;
    float posx = sin(seed*factor)*dist;
    float posz = cos(seed*factor)*dist;
    entity2.get_component<TnaTransform>()->m_position = {posx,0.0f,posz};
    entity2.get_component<TnaTransform>()->m_scale = {0.25f,0.25f,0.25f};
    entity2.get_component<TnaRenderMeshData>()->m_material.m_color = {0.0f, 1.0f, 0.0f};
    entity2.add_reference("parent", entity1);
  }
}

void
create_player(TnaGameState* state)
{
}
  
} /* tna */ 
