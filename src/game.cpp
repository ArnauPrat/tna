


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
  m_game_height(height*CMS_TO_INCHES) {
  }

void Game::on_app_start() {

  glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), 
                               glm::vec3(0.0f, 0.0f, 0.0f), 
                               glm::vec3(0.0f, 0.0f, 1.0f));
  rendering::set_camera(view);

  furious::Entity entity = furious::create_entity(p_database);
  entity.add_component<Transform>(glm::vec3{0.0, 0.0, 0.0});
  entity.add_component<Mesh>("models/cube.obj");
}

void Game::on_app_finish() {


}

void Game::on_frame_update() {

  /*static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

  glm::mat4 model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  */

  //rendering::render_mesh(mesh_data, model);
  
  p_workload->run(1.0, p_database);

}

void Game::on_key_event(GLFWwindow* window, 
                        int key, 
                        int scancode, 
                        int action, 
                        int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

}
