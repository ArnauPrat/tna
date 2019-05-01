



#ifndef _TNA_PREFABS_H_
#define _TNA_PREFABS_H_ value

#include "engine/engine.h"

namespace tna
{

struct TnaGameState;

TnaEntity
create_unit(uint32_t size,
            const std::string& mesh,
            const TnaVector3& position,
            TnaGameState* state);

  
} /* tna */ 

#endif /* ifndef _TNA_PREFABS_H_ */
