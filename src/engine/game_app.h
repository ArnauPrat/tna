


#ifndef _TNA_GAME_APP_H_
#define _TNA_GAME_APP_H_ 

#include "common.h"
#include "entity.h"
#include "game_state.h"

namespace tna 
{

struct GameState;

class GameApp 
{
public:

  GameApp(){};
  virtual ~GameApp(){};

  virtual void 
  on_app_start() = 0;

  virtual void 
  on_app_finish() = 0;

  virtual void 
  on_frame_start(float delta) = 0;

  virtual void 
  on_frame_end() = 0;

  virtual void 
  on_key_event(GLFWwindow* window, 
               int32_t key, 
               int32_t scancode, 
               int32_t action, 
               int32_t mods) = 0;

  virtual void 
  on_cursor_position(GLFWwindow* window,
                     double xpos,
                     double ypos) = 0;

  virtual void 
  on_mouse_button(GLFWwindow* window, 
                  int32_t button, 
                  int32_t action, 
                  int32_t mods) = 0;

  GameState m_state;
};

}

#endif /* ifndef _GAME_APP_H_ */
