


#include "scene.h"
#include "engine/game_state.h"
#include "engine/components/render_mesh_data.h"
#include "engine/components/transform.h"
#include "engine/resources/resources.h"

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

      terrain.get_component<Transform>()->m_scale.x = TILE_HALF_EDGE;
      terrain.get_component<Transform>()->m_scale.y = TILE_HALF_EDGE;
      terrain.get_component<Transform>()->m_scale.z = TILE_HALF_EDGE;

      terrain.get_component<Transform>()->m_position.x  = i*TILE_HALF_EDGE*2.0 - TILE_HALF_EDGE*NUM_TILES;
      terrain.get_component<Transform>()->m_position.y -= TILE_HALF_EDGE;
      terrain.get_component<Transform>()->m_position.z  = j*TILE_HALF_EDGE*2.0 - TILE_HALF_EDGE*NUM_TILES;

      FURIOUS_ADD_COMPONENT(&terrain, RenderMeshData);

      terrain.get_component<RenderMeshData>()->p_mesh_data = mesh_registry->load("models/cube.obj");
      terrain.get_component<RenderMeshData>()->m_material.m_color.r = 0.0;
      terrain.get_component<RenderMeshData>()->m_material.m_color.g = 0.0;
      terrain.get_component<RenderMeshData>()->m_material.m_color.b = 1.0;
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
