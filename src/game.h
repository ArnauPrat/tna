

#ifndef _TNA_SYSTEMS_H_
#define _TNA_SYSTEMS_H_

#include "engine/game_app.h"

class GLFWWindow;

namespace tna 
{

class Game : public GameApp 
{
public:
  Game(int32_t width, 
       int32_t height);

  ~Game() = default;

  virtual void 
  on_app_start();

  virtual void 
  on_app_finish();

  virtual void 
  on_frame_update(float delta);

  virtual void 
  on_key_event(GLFWwindow* window, 
               int key, 
               int scancode, 
               int action, 
               int mods);

  virtual void 
  on_cursor_position(GLFWwindow* window,
                     double xpos,
                     double ypos);

  virtual void 
  on_mouse_button(GLFWwindow* window, 
                  int button, 
                  int action, 
                  int mods);

private:

  void
  create_camera();

  int32_t m_game_width;
  int32_t m_game_height;

};

}

#endif /* ifndef _TNA_SYSTEMS_H_H */



