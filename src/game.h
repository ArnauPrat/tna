

#ifndef _TNA_SYSTEMS_H_
#define _TNA_SYSTEMS_H_

#include "engine/game_app.h"

class GLFWwindow;

namespace tna 
{

class Game : public GameApp 
{
public:
  Game(int32_t width, 
       int32_t height);

  ~Game() = default;

  virtual void 
  on_app_start(GLFWwindow* window);

  virtual void 
  on_app_finish();

  virtual void 
  on_frame_start(float delta);

  virtual void 
  on_frame_end();

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

  bool m_forwards_camera;
  bool m_backwards_camera;
  bool m_strafe_left_camera;
  bool m_strafe_right_camera;

  double m_mouse_old_pos_x;
  double m_mouse_old_pos_y;

  double m_mouse_current_pos_x;
  double m_mouse_current_pos_y;
};

}

#endif /* ifndef _TNA_SYSTEMS_H_H */



