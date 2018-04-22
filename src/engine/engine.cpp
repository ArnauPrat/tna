


#include "common.h"
#include "config.h"
#include "game_app.h"
#include <fstream>
#include <furious/furious.h>


namespace tna {

Log*        log     = nullptr;
GLFWwindow* window  = nullptr;
GameApp*    current_app = nullptr;

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
  Config config;
  if(file_exists("./config.ini")) {
    config = load_config("./config.ini");
  } 

  // Initializing Furious
  furious::init();

  // Creating Window
  glfwInit();

  window = glfwCreateWindow(config.m_viewport_width, 
                            config.m_viewport_height, 
                            "TNA", 
                            config.m_fullscreen ? glfwGetPrimaryMonitor() : NULL,
                            NULL);
  if (!window) {
    log->error("Error creating window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  
}

void terminate() {

  glfwDestroyWindow(window);
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

}

}
