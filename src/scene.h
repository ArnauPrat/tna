


#ifndef _TNA_SCENE_H_
#define _TNA_SCENE_H_ value



namespace tna
{

struct GameState;

void
create_terrain(GameState* state);

void
create_buildings(GameState* state);

void
create_cars(GameState* state);

void
create_player(GameState* state);
  
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
