


#ifndef _TNA_SCENE_H_
#define _TNA_SCENE_H_ value



namespace tna
{

struct game_state_t;

void
create_terrain(game_state_t* state);

void
create_buildings(game_state_t* state);

void
create_cars(game_state_t* state);

void
create_player(game_state_t* state);
  
} /* tna */ 

#endif /* ifndef _TNA_SCENE_H_ */
