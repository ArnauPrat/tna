

#include "game.h"
#include "engine/rendering/renderer.h"
#include "engine/math/math_tools.h"
#include "tnasdk/definitions.h"

#include "engine/components/transform.h"
#include "engine/components/mesh.h"

#include <time.h>
#include <stdlib.h>

namespace tna 
{

Game::Game(int32_t width, 
           int32_t height) : 
m_game_width(width*CMS_TO_INCHES),
m_game_height(height*CMS_TO_INCHES)
{
}

void Game::on_app_start() 
{

  Entity entity1 = create_entity();
  FURIOUS_ADD_COMPONENT(&entity1, Mesh, "models/cube.obj");
  entity1.get_component<Transform>()->m_position = {0.0f,0.0f,0.0f};

  srand(time(NULL));

  double factor = 3.1416f / 180.0f;
  for(uint32_t i = 0; i < 20; ++i)
  {
    Entity entity2 = create_entity();
    FURIOUS_ADD_COMPONENT(&entity2, Mesh, "models/cube.obj");
    int seed = rand() % 360;
    float posx = sin(seed*factor)*3.0;
    float posz = cos(seed*factor)*3.0;
    entity2.get_component<Transform>()->m_position = {posx,0.0f,posz};
    entity2.get_component<Transform>()->m_scale = {0.25f,0.25f,0.25f};
    entity2.add_reference("parent", entity1);
  }

}

void Game::on_app_finish() 
{


}

void Game::on_frame_update(float delta) 
{
}

void Game::on_key_event(GLFWwindow* window, 
                        int key, 
                        int scancode, 
                        int action, 
                        int mods) 
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Game::on_cursor_position(GLFWwindow* window,
                        double xpos,
                        double ypos) 
{

}

void Game::on_mouse_button(GLFWwindow* window, 
                     int button, 
                     int action, 
                     int mods) 
{

}


}
