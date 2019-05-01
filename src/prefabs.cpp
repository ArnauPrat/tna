



#include "prefabs.h"
#include "game_references.h"
#include "components/soldier.h"
#include "components/laggy_follower.h"
#include "engine/components/render_mesh_data.h"
#include "engine/components/transform.h"
#include "engine/components/transform_matrix.h"
#include "engine/tools/references.h"


namespace  tna
{



TnaEntity
create_unit(uint32_t size,
            const std::string& mesh,
            const TnaVector3& position,
            TnaGameState* state)
{

  TnaEntity unit = create_entity(state); 

  TnaTransform* transform = FURIOUS_GET_COMPONENT(unit, TnaTransform);
  transform->m_position.x = position.x;
  transform->m_position.y = position.y;
  transform->m_position.z = position.z;
  transform->m_dirty = true;

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
      FURIOUS_ADD_COMPONENT(soldiers[count], 
                            TnaRenderMeshData, 
                            TnaRenderMobilityType::E_STATIC, mesh);

      float factor =  0.5*(((uint32_t)rand()) / (double) UINT_MAX);
      FURIOUS_ADD_COMPONENT(soldiers[count], 
                            LaggyFollower,
                            TnaVector3{0.0f,0.0f,0.0f},
                            TnaVector3{xpos, ypos, zpos},
                            (factor+0.5f)*(next_row+1));

      TnaTransform* transform = FURIOUS_GET_COMPONENT(soldiers[count], TnaTransform);
      transform->m_position.x = xpos;
      transform->m_position.y = ypos;
      transform->m_position.z = zpos;
      transform->m_dirty = true;

      FURIOUS_ADD_REFERENCE(soldiers[count], 
                            TNA_REF_PARENT, 
                            unit);

      if(champion_idx == count)
      {
        TnaRenderMeshData* champion_mesh = FURIOUS_GET_COMPONENT(soldiers[count], TnaRenderMeshData);
        TnaMaterialDescriptor mat_desc;
        p_rendering_scene->get_material(champion_mesh->m_handler,
                                        &mat_desc);
        mat_desc.m_color = {1.0, 0.0, 0.0};
        p_rendering_scene->set_material(champion_mesh->m_handler,

                                        mat_desc);
      }
    }
    ++next_row;
  }


  delete [] soldiers;
  return unit;
}
  
} /*  tna */ 
