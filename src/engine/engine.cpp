
#include "common.h"
#include "config.h"
#include "game_app.h"
#include "rendering/rendering.h"
#include "resources/resources.h"
#include <chrono>
#include <fstream>
#include <glm/glm.hpp>
#include <furious/furious.h>

#include "components/proj_view_matrix.h"
#include "components/viewport.h"


namespace tna 
{

Log*               log     = nullptr;

GLFWwindow*        p_window  = nullptr;
GameApp*           p_current_app = nullptr;
Config             m_config;
furious::Database* p_database = nullptr;;


static void 
key_callback(GLFWwindow* window, 
             int key, 
             int scancode, 
             int action, 
             int mods)
{
  p_current_app->on_key_event(window, 
                              key, 
                              scancode, 
                              action, 
                              mods);
}

static 
void cursor_position_callback(GLFWwindow* window, 
                              double xpos, 
                              double ypos)
{
  p_current_app->on_cursor_position(window, 
                                    xpos, 
                                    ypos);
}


void 
mouse_button_callback(GLFWwindow* window, 
                      int button, 
                      int action, 
                      int mods)
{
  p_current_app->on_mouse_button(window, 
                                 button, 
                                 action, 
                                 mods);
}

static bool 
file_exists(const std::string& filename) 
{
  std::ifstream file(filename);
  return file.good();
}

void
initialize() 
{
  // Initializing loging system
  log = new Log("./logfile.log");

  // Reading engine's config
  if(file_exists("./config.ini")) 
  {
    load_config("./config.ini", &m_config);
  } 

  init_resources();
  register_directory("./");

  // Creating Window
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  p_window = glfwCreateWindow(m_config.m_viewport_width, 
                              m_config.m_viewport_height, 
                              "TNA", 
                              m_config.m_fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                              nullptr);
  if (!p_window) 
  {
    log->error("Error creating window");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  glfwMakeContextCurrent(p_window);
  glfwSetKeyCallback(p_window, key_callback);
  glfwSetMouseButtonCallback(p_window, mouse_button_callback);
  glfwSetCursorPosCallback(p_window, cursor_position_callback);
  init_renderer(&m_config, p_window);
  return;
}

void
terminate() 
{
  if(p_current_app != nullptr) 
  {
    p_current_app->on_app_finish();
  }

  terminate_renderer();
  

  if(p_window != nullptr) 
  {
    glfwDestroyWindow(p_window);
  }
  glfwTerminate();

  terminate_resources();
  delete log;
  return;
}

void
run(GameApp* game_app) 
{
  static auto start_time = std::chrono::high_resolution_clock::now();

  p_database = new furious::Database();
  p_database->start_webserver("localhost", "8080");

  /// ADDING GLOBALS TO FURIOUS
  FURIOUS_CREATE_GLOBAL((p_database),
                        ProjViewMatrix);

  FURIOUS_CREATE_GLOBAL((p_database),
                        Viewport,
                        m_config.m_viewport_width,
                        m_config.m_viewport_height,
                        0.1f,
                        1000.0f,
                        60.0f);

  // Initializing Furious
  furious::__furious_init(p_database);

  p_current_app = game_app;
  p_current_app->m_state.p_database = p_database;
  p_current_app->m_state.p_window = p_window;

  p_current_app->on_app_start();
  while (!glfwWindowShouldClose(p_window)) 
  {
    // Keep running
    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
    start_time = current_time;

    glfwPollEvents();
    begin_frame();
    p_current_app->on_frame_start(time);
    furious::__furious_frame(time, p_database);
    p_current_app->on_frame_end();
    end_frame();
  }

  p_current_app->on_app_finish();
  p_current_app = nullptr;

  if(p_database)
  {
    furious::__furious_release();
    delete p_database;
  }
  return;
}

}
