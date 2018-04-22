

#ifndef _TNA_SYSTEMS_H_
#define _TNA_SYSTEMS_H_

#include <furious/furious.h>
#include "engine/engine.h"


namespace tna {

class Game : public GameApp {
public:
  Game(int32_t width, 
       int32_t height);

  ~Game() = default;

  virtual void on_app_start();

  virtual void on_app_finish();

  virtual void on_frame_update();

  virtual void on_key_event(GLFWwindow* window, 
                            int key, 
                            int scancode, 
                            int action, 
                            int mods);

private:

  int32_t m_game_width;
  int32_t m_game_height;


};

}

#endif /* ifndef _TNA_SYSTEMS_H_H */



