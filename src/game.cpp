


#include "engine/engine.h"
#include "game.h"
#include "tnasdk/definitions.h"
#include <chrono>

#include "engine/components/transform.h"
#include "engine/components/mesh.h"


namespace tna {

Game::Game(int32_t width, 
           int32_t height) : 
  m_game_width(width*CMS_TO_INCHES),
  m_game_height(height*CMS_TO_INCHES),
  m_entity(furious::create_entity(p_database)){
  }

void Game::on_app_start() {

  glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), 
                               glm::vec3(0.0f, 0.0f, 0.0f), 
                               glm::vec3(0.0f, 0.0f, 1.0f));

  rendering::set_camera(view);

  m_entity.add_component<Transform>(glm::vec3{0.0, 0.0, 0.0});
  m_entity.add_component<Mesh>("models/cube.obj");
}

void Game::on_app_finish() {


}

void Game::on_frame_update(float delta) {
  m_entity.get_component<Transform>()->m_rotation.z = delta*glm::radians(90.0f);
  p_workload->run(delta, p_database);
}

void Game::on_key_event(GLFWwindow* window, 
                        int key, 
                        int scancode, 
                        int action, 
                        int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Game::on_cursor_position(GLFWwindow* window,
                        double xpos,
                        double ypos) {

}

void Game::on_mouse_button(GLFWwindow* window, 
                     int button, 
                     int action, 
                     int mods) {
}

}
