

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
m_forwards_unit(false), 
m_mouse_old_pos_x(0.0),
m_mouse_old_pos_y(0.0),
m_mouse_delta_pos_x(0.0),
m_mouse_delta_pos_y(0.0)
{
}

void Game::on_app_start() 
{

  create_camera(&m_state);

  TnaViewport* viewport = FURIOUS_FIND_GLOBAL(m_state.p_database, TnaViewport); 
  viewport->m_far = 10000.0f;

  // SETTING CAMERA POSITION
  TnaFPSCamera* fps_camera = FURIOUS_FIND_GLOBAL(m_state.p_database, TnaFPSCamera);
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

  p_rendering_scene->set_clear_color(TNA_COLOR_BLUE_SKY);



  // CREATING SCENE
  create_terrain(&m_state);
  //create_buildings(&m_state);
  //create_cars(&m_state);
  //create_player(&m_state);

  for(uint32_t i = 0; i < 20; ++i)
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
}

void Game::on_app_finish() 
{


}

void Game::on_frame_start(float delta) 
{
  TnaFPSCamera* camera = m_state.p_database->find_global<TnaFPSCamera>();
  if(m_forwards_camera)
  {
    camera->forward(delta);
  }

  if(m_backwards_camera)
  {
    camera->forward(-delta);
  }

  if(m_strafe_left_camera)
  {
    camera->strafe(-delta);
  }

  if(m_strafe_right_camera)
  {
    camera->strafe(delta);
  }

  if(m_forwards_unit)
  {
    for(uint32_t i = 0; i < m_units.size(); ++i)
    {
      TnaTransform* transform = FURIOUS_GET_COMPONENT(m_units[i],TnaTransform);
      transform->m_position.z += delta*0.1;
      transform->m_dirty = true;
    }
  }

  camera->pitch(m_mouse_delta_pos_y);
  camera->yaw(m_mouse_delta_pos_x);
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
      case GLFW_KEY_UP:
        if (action == GLFW_PRESS)
        {
          m_forwards_unit = true;
        }
        else if(action == GLFW_RELEASE)
        {
          m_forwards_unit = false;
        }
        break;
    }
  }
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
