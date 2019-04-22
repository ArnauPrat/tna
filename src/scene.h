


#ifndef _TNA_SCENE_H_
#define _TNA_SCENE_H_ value



namespace tna
{

struct TnaGameState;

void
create_terrain(TnaGameState* state);

void
create_buildings(TnaGameState* state);

void
create_cars(TnaGameState* state);

void
create_player(TnaGameState* state);
  
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
