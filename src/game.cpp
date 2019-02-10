


#include "game.h"
#include "engine/math/math_tools.h"
#include "tnasdk/definitions.h"

#include "engine/components/transform.h"
#include "engine/components/mesh.h"

#include "components/rotation.h"

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
  Matrix4 view = look_at(Vector3(0.0f, 5.0f, 5.0f), 
                         Vector3(0.0f, 0.0f, 0.0f), 
                         Vector3(0.0f, 1.0f, 0.0f));

  set_camera(&view);

  //Entity entity = create_entity();
  //FURIOUS_ADD_COMPONENT(&entity, Mesh, "models/cube.obj");
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
