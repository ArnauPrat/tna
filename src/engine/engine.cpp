


#include "common.h"
#include "config.h"
#include "game_app.h"
#include "rendering/rendering.h"
#include <fstream>
#include <furious/furious.h>
#include <glm/glm.hpp>


namespace tna {

GLFWwindow* window  = nullptr;
Log*        log     = nullptr;
GameApp*    current_app = nullptr;
Config      config;


static void key_callback(GLFWwindow* window, 
                         int key, 
                         int scancode, 
                         int action, 
                         int mods)
{
  current_app->on_key_event(window, key, scancode, action, mods);
}

static bool file_exists(const std::string& filename) {
  std::ifstream file(filename);
  return file.good();
}

void initialize() {

  // Initializing loging system
  log = new Log("./logfile.log");

  // Reading engine's config
  if(file_exists("./config.ini")) {
    config = load_config("./config.ini");
  } 

  // Initializing Furious
  furious::init();

  // Creating Window
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(config.m_viewport_width, 
                            config.m_viewport_height, 
                            "TNA", 
                            config.m_fullscreen ? glfwGetPrimaryMonitor() : NULL,
                            NULL);
  if (!window) {
    throw std::runtime_error("Error creating window");
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  init_rendering(config, window);

}

void terminate() {

  if(current_app != nullptr) {
    current_app->on_app_finish();
  }

  terminate_rendering();

  if(window != nullptr) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
  furious::release();
  delete log;

}

void run(GameApp* game_app) {

  current_app = game_app;
  current_app->on_app_start();

  while (!glfwWindowShouldClose(window))
  {
    // Keep running
    glfwPollEvents();
    current_app->on_frame_update();
    glfwSwapBuffers(window);
  }

  current_app->on_app_finish();
  current_app = nullptr;

}

}
