


#include "scene.h"
#include "engine/game_state.h"
#include "engine/components/render_mesh_data.h"
#include "engine/components/transform.h"
#include "engine/resources/resources.h"
#include "engine/tools/colors.h"

namespace tna
{

void
create_terrain(GameState* state)
{
  uint32_t NUM_TILES = 100;
  float TILE_HALF_EDGE = 5.0;
  for(uint32_t i = 0; i < NUM_TILES; ++i)
  {
    for(uint32_t j = 0; j < NUM_TILES; ++j)
    {
      Entity terrain = create_entity(state);

      Transform* transform = terrain.get_component<Transform>();
      transform->m_scale.x = TILE_HALF_EDGE;
      transform->m_scale.y = 5.0f;
      transform->m_scale.z = TILE_HALF_EDGE;

      transform->m_position.x  = i*TILE_HALF_EDGE*2.0 - TILE_HALF_EDGE*NUM_TILES;
      transform->m_position.y -= 5.0f;
      transform->m_position.z  = j*TILE_HALF_EDGE*2.0 - TILE_HALF_EDGE*NUM_TILES;

      FURIOUS_ADD_COMPONENT(&terrain, RenderMeshData);

      RenderMeshData* render_mesh_data = terrain.get_component<RenderMeshData>();
      render_mesh_data->p_mesh_data = mesh_registry->load("models/cube.obj");
      render_mesh_data->m_material.m_color = TNA_COLOR_LIGHT_GREY_2;
    }
  }
}

void
create_buildings(GameState* state)
{
}

void
create_cars(GameState* state)
{
}

void
create_player(GameState* state)
{
}
  
} /* tna */ 
