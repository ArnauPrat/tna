



#include "prefabs.h"
#include "game_references.h"
#include "components/soldier.h"
#include "components/unit_member.h"
#include "engine/components/render_mesh_data.h"
#include "engine/components/transform.h"
#include "engine/components/transform_matrix.h"
#include "engine/tools/references.h"


namespace  tna
{



TnaEntity
create_unit(uint32_t size,
            const std::string& mesh,
            const vector3_t& position,
            game_state_t* state)
{

  TnaEntity unit = create_entity(state); 

  transform_t* unit_transform = FURIOUS_GET_COMPONENT(unit, transform_t);
  unit_transform->m_position.x = position.x;
  unit_transform->m_position.y = position.y;
  unit_transform->m_position.z = position.z;
  unit_transform->m_dirty = true;

  TnaEntity* soldiers = new TnaEntity[size];
  constexpr int32_t MAX_RANKS = 5;
  constexpr int32_t SEPARATION = 3;
  uint32_t count = 0;
  uint32_t champion_idx = size / 2;
  if(size > 3)
  {
    champion_idx = 2;
  }
  int32_t next_row = 0;
  while(count < size) 
  {
    for(int32_t j = 0; (j < MAX_RANKS) && (count < size); ++j, ++count)
    {
      float xpos = j*SEPARATION - (MAX_RANKS-1)*SEPARATION/2;
      float ypos = 1.0;
      float zpos = -next_row*SEPARATION;

      soldiers[count] = create_entity(state);
      render_mesh_data_t* component = FURIOUS_ADD_COMPONENT(soldiers[count], 
                                                            render_mesh_data_t);
      render_mesh_data_init(component, "models/cube.obj");

      FURIOUS_ADD_COMPONENT(soldiers[count], 
                            UnitMember,
                            vector3_t{xpos, ypos, zpos},
                            vector3_t{xpos, ypos, zpos},
                            1.0f,
                            next_row,
                            j);

      transform_t* transform = FURIOUS_GET_COMPONENT(soldiers[count], transform_t);
      transform->m_position.x = unit_transform->m_position.x + xpos;
      transform->m_position.y = unit_transform->m_position.y + ypos;
      transform->m_position.z = unit_transform->m_position.z + zpos;
      transform->m_dirty = true;

      FURIOUS_ADD_REFERENCE(soldiers[count], 
                            TNA_GAME_REF_BELONGS_TO_UNIT, 
                            unit);

      if(champion_idx == count)
      {
        render_mesh_data_t* champion_mesh = FURIOUS_GET_COMPONENT(soldiers[count], render_mesh_data_t);
        material_desc_t mat_desc;
        rendering_scene_get_material(p_rendering_scene,
                                     champion_mesh->m_handler,
                                     &mat_desc);

        mat_desc.m_color = {1.0, 0.0, 0.0};
        rendering_scene_set_material(p_rendering_scene,
                                     champion_mesh->m_handler,
                                     mat_desc);
      }
    }
    ++next_row;
  }

  delete [] soldiers;
  return unit;
}
  
} /*  tna */ 
