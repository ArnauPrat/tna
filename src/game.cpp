

#include "game.h"
#include "scene.h"
#include "prefabs.h"
#include "engine/engine.h"
#include "engine/gui/imgui.h"
#include "engine/components/fps_camera.h"
#include "engine/components/viewport.h"
#include "engine/rendering/renderer.h"
#include "engine/math/math_tools.h"
#include "engine/tools/colors.h"
#include "tnasdk/definitions.h"

#include "engine/components/transform.h"
#include "engine/components/fps_camera.h"
#include "engine/components/render_mesh_data.h"
#include "components/rotation_speed.h"

#include <time.h>
#include <stdlib.h>

namespace tna 
{

Game::Game(int32_t width, 
           int32_t height) : 
m_game_width(width*CMS_TO_INCHES),
m_game_height(height*CMS_TO_INCHES),
m_forwards_camera(false),
m_backwards_camera(false),
m_strafe_left_camera(false),
m_strafe_right_camera(false),
m_mouse_old_pos_x(0.0),
m_mouse_old_pos_y(0.0),
m_mouse_delta_pos_x(0.0),
m_mouse_delta_pos_y(0.0)
{
}

void Game::on_app_start() 
{

  create_camera(&m_state);

  viewport_t* viewport = FURIOUS_FIND_GLOBAL(m_state.p_database, viewport_t); 
  viewport->m_far = 10000.0f;

  // SETTING CAMERA POSITION
  fps_camera_t* fps_camera = FURIOUS_FIND_GLOBAL(m_state.p_database, fps_camera_t);
  fps_camera->m_eye = {0.0f, 100.0f, 100.0f};
  fps_camera->m_speed = 200.0f;

  fps_camera->m_pitch = radians(-45.0f);
  fps_camera->m_yaw_speed = radians(10.0f);
  fps_camera->m_pitch_speed = radians(10.0f);

  // INITIALIZING CURRENT MOUSE POSITION
  glfwGetCursorPos(m_state.p_window, 
                   &m_mouse_old_pos_x, 
                   &m_mouse_old_pos_y);

  m_mouse_delta_pos_x = 0.0;
  m_mouse_delta_pos_y = 0.0;

  rendering_scene_set_clear_color(p_rendering_scene,
                                  TNA_COLOR_BLUE_SKY);



  // CREATING SCENE
  create_terrain(&m_state);
  //create_buildings(&m_state);
  create_cars(&m_state);
  //create_player(&m_state);

  /*for(uint32_t i = 0; i < 20; ++i)
  {
    for(uint32_t j = 0; j < 20; ++j)
    {
      TnaEntity unit = create_unit(20,
                                   "models/cube.obj",
                                   {i*30.0f, 0.0f, j*30.0f},
                                   &m_state);
      m_units.append(unit);
    }
  }
  */
}

void Game::on_app_finish() 
{


}

void Game::on_frame_start(float delta) 
{
  fps_camera_t* camera = m_state.p_database->find_global<fps_camera_t>();
  if(m_forwards_camera)
  {
    fps_camera_forward(camera,delta);
  }

  if(m_backwards_camera)
  {
    fps_camera_forward(camera, -delta);
  }

  if(m_strafe_left_camera)
  {
    fps_camera_strafe(camera,-delta);
  }

  if(m_strafe_right_camera)
  {
    fps_camera_strafe(camera, delta);
  }

  fps_camera_pitch(camera, m_mouse_delta_pos_y);
  fps_camera_yaw(camera, m_mouse_delta_pos_x);

  m_mouse_delta_pos_x = 0;
  m_mouse_delta_pos_y = 0;
}

void Game::on_frame_end()
{
}


void Game::on_key_event(GLFWwindow* window, 
                        int key, 
                        int scancode, 
                        int action, 
                        int mods) 
{
  // Free mode
  if(!is_edit_mode())
  {
    switch(key)
    {
      case GLFW_KEY_W:
        if (action == GLFW_PRESS)
        {
          m_forwards_camera = true;
        }
        else if(action == GLFW_RELEASE)
        {
          m_forwards_camera = false;
        }
        break;

      case GLFW_KEY_S:
        if (action == GLFW_PRESS)
        {
          m_backwards_camera = true;
        }
        else if(action == GLFW_RELEASE)
        {
          m_backwards_camera = false;
        }
        break;

      case GLFW_KEY_A:
        if (action == GLFW_PRESS)
        {
          m_strafe_left_camera = true;
        }
        else if(action == GLFW_RELEASE)
        {
          m_strafe_left_camera = false;
        }
        break;

      case GLFW_KEY_D:
        if (action == GLFW_PRESS)
        {
          m_strafe_right_camera = true;
        }
        else if(action == GLFW_RELEASE)
        {
          m_strafe_right_camera = false;
        }
        break;
    }
  }

  // General key commands
  switch(key)
  {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
    case GLFW_KEY_G:
      if (action == GLFW_PRESS)
      {
        toggle_gui();
      }
      break;
    case GLFW_KEY_E:
      if (action == GLFW_PRESS)
      {
        toggle_edit_mode();
        glfwGetCursorPos(m_state.p_window, 
                         &m_mouse_old_pos_x, 
                         &m_mouse_old_pos_y);

        m_mouse_delta_pos_x = 0;
        m_mouse_delta_pos_y = 0;
        m_forwards_camera = false;
        m_backwards_camera = false;
        m_strafe_left_camera = false;
        m_strafe_right_camera = false;
      }
  }
}

void Game::on_cursor_position(GLFWwindow* window,
                              double xpos,
                              double ypos) 
{
  if(!is_edit_mode())
  {
    m_mouse_delta_pos_x += (xpos - m_mouse_old_pos_x);
    m_mouse_delta_pos_y += (ypos - m_mouse_old_pos_y);
    m_mouse_old_pos_x = xpos;
    m_mouse_old_pos_y = ypos;
  }
}

void Game::on_mouse_button(GLFWwindow* window, 
                           int button, 
                           int action, 
                           int mods) 
{

}

}
