

#include "game.h"
#include "scene.h"
#include "engine/components/fps_camera.h"
#include "engine/rendering/renderer.h"
#include "engine/math/math_tools.h"
#include "tnasdk/definitions.h"

#include "engine/components/transform.h"
#include "engine/components/fps_camera.h"

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
m_mouse_current_pos_x(0.0),
m_mouse_current_pos_y(0.0)
{
}

void Game::on_app_start() 
{

  create_camera(&m_state);

  FPSCamera* fps_camera = FURIOUS_FIND_GLOBAL(m_state.p_database, FPSCamera);
  fps_camera->m_eye.y = 2.0f;

  glfwSetInputMode(m_state.p_window, 
                   GLFW_CURSOR,
                   GLFW_CURSOR_DISABLED);

  glfwSetCursorPos(m_state.p_window, 
                   m_game_width/2.0, 
                   m_game_height/2.0);

  glfwGetCursorPos(m_state.p_window, 
                   &m_mouse_old_pos_x, 
                   &m_mouse_old_pos_y);

  m_mouse_current_pos_x = m_mouse_old_pos_x;
  m_mouse_current_pos_y = m_mouse_old_pos_y;

  set_clear_color({0.2f, 0.2f, 0.2f});

  create_terrain(&m_state);
  create_buildings(&m_state);
  create_cars(&m_state);
  create_player(&m_state);


  /*Entity entity1 = create_entity(&m_state);
  FURIOUS_ADD_COMPONENT(&entity1, Mesh, "models/cube.obj");
  entity1.get_component<Transform>()->m_position = {0.0f,0.0f,0.0f};
  entity1.get_component<Mesh>()->m_color = {1.0f, 0.0f, 0.0f};

  srand(time(NULL));

  double factor = 3.1416f / 180.0f;
  for(uint32_t i = 0; i < 20; ++i)
  {
    Entity entity2 = create_entity(&m_state);
    FURIOUS_ADD_COMPONENT(&entity2, Mesh, "models/cube.obj");
    int seed = rand() % 360;
    float posx = sin(seed*factor)*3.0;
    float posz = cos(seed*factor)*3.0;
    entity2.get_component<Transform>()->m_position = {posx,0.0f,posz};
    entity2.get_component<Transform>()->m_scale = {0.25f,0.25f,0.25f};
    entity2.get_component<Mesh>()->m_color = {0.0f, 1.0f, 0.0f};
    entity2.add_reference("parent", entity1);
  }
  */

}

void Game::on_app_finish() 
{


}

void Game::on_frame_start(float delta) 
{
  FPSCamera* camera = m_state.p_database->find_global<FPSCamera>();
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

  int32_t delta_x = m_mouse_current_pos_x - m_mouse_old_pos_x;
  int32_t delta_y = m_mouse_current_pos_y - m_mouse_old_pos_y;
  m_mouse_old_pos_x = m_mouse_current_pos_x;
  m_mouse_old_pos_y = m_mouse_current_pos_y;

  camera->pitch(delta_y);
  camera->yaw(delta_x);
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
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (key == GLFW_KEY_W)
  {
    if (action == GLFW_PRESS)
    {
      m_forwards_camera = true;
    }
    else if(action == GLFW_RELEASE)
    {
      m_forwards_camera = false;
    }
  }

  if (key == GLFW_KEY_S)
  {
    if (action == GLFW_PRESS)
    {
      m_backwards_camera = true;
    }
    else if(action == GLFW_RELEASE)
    {
      m_backwards_camera = false;
    }
  }

  if (key == GLFW_KEY_A)
  {
    if (action == GLFW_PRESS)
    {
      m_strafe_left_camera = true;
    }
    else if(action == GLFW_RELEASE)
    {
      m_strafe_left_camera = false;
    }

  }

  if (key == GLFW_KEY_D)
  {
    if (action == GLFW_PRESS)
    {
      m_strafe_right_camera = true;
    }
    else if(action == GLFW_RELEASE)
    {
      m_strafe_right_camera = false;
    }
  }
}

void Game::on_cursor_position(GLFWwindow* window,
                              double xpos,
                              double ypos) 
{
  m_mouse_old_pos_x = m_mouse_current_pos_x;
  m_mouse_old_pos_y = m_mouse_current_pos_y;
  m_mouse_current_pos_x = xpos;
  m_mouse_current_pos_y = ypos;
}

void Game::on_mouse_button(GLFWwindow* window, 
                           int button, 
                           int action, 
                           int mods) 
{

}

}
