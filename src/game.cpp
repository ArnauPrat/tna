


#include "engine/common.h"
#include "game.h"
#include "tnasdk/definitions.h"



namespace tna {

Game::Game(int32_t width, 
           int32_t height) : 
  m_game_width(width*CMS_TO_INCHES),
  m_game_height(height*CMS_TO_INCHES) {
  }

void Game::on_app_start() {

}

void Game::on_app_finish() {


}

void Game::on_frame_update() {
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
