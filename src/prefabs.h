



#ifndef _TNA_PREFABS_H_
#define _TNA_PREFABS_H_ value

#include "engine/engine.h"

namespace tna
{

struct game_state_t;

entity_t
create_unit(uint32_t size,
            const std::string& mesh,
            const vector3_t& position,
            game_state_t* state);

  
} /* tna */ 

#endif /* ifndef _TNA_PREFABS_H_ */
