



#include "prefabs.h"
#include "game_references.h"
#include "engine/components/render_mesh_data.h"
#include "engine/components/transform.h"
#include "engine/tools/references.h"


namespace  tna
{



TnaEntity
create_unit(uint32_t size,
            const std::string& mesh,
            TnaGameState* state)
{

  TnaEntity* soldiers = new TnaEntity[size];
  constexpr int32_t MAX_RANKS = 5;
  constexpr int32_t SEPARATION = 3;
  uint32_t count = 0;
  int32_t next_row = 0;
  while(count < size) 
  {
    for(int32_t j = 0; (j < MAX_RANKS) && (count < size); ++j, ++count)
    {
      float xpos = j*SEPARATION - (MAX_RANKS-1)*SEPARATION/2;
      float zpos = -next_row*SEPARATION;

      soldiers[count] = create_entity(state);
      FURIOUS_ADD_COMPONENT(soldiers[count], 
                            TnaRenderMeshData, 
                            TnaRenderMobilityType::E_STATIC, mesh);

      TnaTransform* transform = FURIOUS_GET_COMPONENT(soldiers[count], TnaTransform);
      transform->m_position.x = xpos;
      transform->m_position.y = 1.0f;
      transform->m_position.z = zpos;
      transform->m_dirty = true;
    }
    ++next_row;
  }


  TnaEntity champion; 
  if(size >= 3)
  {
    champion = soldiers[2];
  }
  else
  {
    champion = soldiers[size - 1];
  }

  for(uint32_t i = 0; i < size; ++i)
  {
    if(champion.m_id != soldiers[i].m_id)
    {
      /*FURIOUS_ADD_REFERENCE(soldiers[i], 
                            TNA_REF_PARENT, 
                            champion);
                            */

      FURIOUS_ADD_REFERENCE(soldiers[i], 
                            TNA_GAME_REF_IS_LEAD_BY, 
                            champion);
    }
  }

  delete [] soldiers;

  TnaRenderMeshData* mesh_data = FURIOUS_GET_COMPONENT(champion, TnaRenderMeshData);
  TnaMaterialDescriptor mat_desc;
  p_rendering_scene->get_material(mesh_data->m_handler, &mat_desc);
  mat_desc.m_color = TNA_COLOR_RED;
  p_rendering_scene->set_material(mesh_data->m_handler, mat_desc);

  return champion;
}
  
} /*  tna */ 
