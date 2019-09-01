
#include "common.h"
#include "config.h"
#include "game_app.h"
#include "rendering/rendering.h"
#include "resources/resources.h"
#include "gui/imgui.h"
#include <chrono>
#include <fstream>
#include <glm/glm.hpp>
#include <furious/furious.h>

#include "components/proj_view_matrix.h"
#include "components/viewport.h"
#include "components/transform.h"
#include "components/transform_matrix.h"
#include "components/render_mesh_data.h"


namespace tna 
{

bool                m_show_gui = true;
bool                m_edit_mode = false;
GLFWwindow*         p_window  = nullptr;
TnaGameApp*         p_current_app = nullptr;
config_t            m_config;
furious::Database*  p_database = nullptr;;


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
  log_init("./logfile.log");
  directory_registry_init();

  // Reading engine's config
  if(file_exists("./config.ini")) 
  {
    config_init(&m_config, "./config.ini");
  } 

  init_resources();
  directory_registry_insert("./");

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
    TNA_LOG_ERROR("Error creating window");
    report_error(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR);
  }

  glfwMakeContextCurrent(p_window);
  glfwSetKeyCallback(p_window, key_callback);
  glfwSetMouseButtonCallback(p_window, mouse_button_callback);
  glfwSetCursorPosCallback(p_window, cursor_position_callback);
  glfwSetInputMode(p_window, 
                   GLFW_CURSOR,
                   GLFW_CURSOR_DISABLED);

  glfwSetCursorPos(p_window, 
                   m_config.m_viewport_width/2.0, 
                   m_config.m_viewport_height/2.0);

  init_renderer(&m_config, p_window);
  init_gui();
  rendering_scene_create();

  return;
}

void
terminate() 
{
  if(p_current_app != nullptr) 
  {
    p_current_app->on_app_finish();
  }

  rendering_scene_destroy();
  terminate_gui();
  terminate_renderer();
  

  if(p_window != nullptr) 
  {
    glfwDestroyWindow(p_window);
  }
  glfwTerminate();

  terminate_resources();
  config_release(&m_config);
  directory_registry_release();
  log_release();
  return;
}

void
toggle_gui()
{
  m_show_gui = !m_show_gui;
}

void
toggle_edit_mode()
{
  if(m_edit_mode)
  {
    glfwSetInputMode(p_window, 
                     GLFW_CURSOR,
                     GLFW_CURSOR_DISABLED);
  }
  else
  {
    glfwSetInputMode(p_window, 
                     GLFW_CURSOR,
                     GLFW_CURSOR_NORMAL);

    viewport_t* viewport = FURIOUS_FIND_GLOBAL(p_database, viewport_t);
    glfwSetCursorPos(p_window, 
                     viewport->m_width/2.0, 
                     viewport->m_height/2.0);

  }
  m_edit_mode = !m_edit_mode;
}

bool
is_edit_mode()
{
  return m_edit_mode;
}

void
draw_gui()
{
  ImGui::NewFrame();
  if(m_show_gui)
  {
    ImGui::Begin("TNA");                         
    ImGui::Text("Frame time %.3f ms",
                1000.0f / ImGui::GetIO().Framerate);

    ImGui::Text("FPS %.1f", 
                ImGui::GetIO().Framerate);

    ImGui::End();
  }
  ImGui::Render();
}

void
run(TnaGameApp* game_app) 
{
  static auto start_time = std::chrono::high_resolution_clock::now();

  p_database = new furious::Database();
  p_database->start_webserver("localhost", "8080");

  /// MANUALLY CREATING TABLES WITH REQUIRED MANUAL DESTRUCTORS
  FURIOUS_CREATE_TABLE_DESTRUCTOR((p_database), 
                                  render_mesh_data_t,
                                  render_mesh_data_release);

  /// ADDING GLOBALS TO FURIOUS
  FURIOUS_CREATE_GLOBAL((p_database),
                        projview_matrix_t);

  viewport_t* viewport = FURIOUS_CREATE_GLOBAL((p_database),
                                                viewport_t);
  viewport->m_width = m_config.m_viewport_width;
  viewport->m_height = m_config.m_viewport_height;

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
    begin_frame(p_rendering_scene);
    p_current_app->on_frame_start(time);
    furious::__furious_frame(time, 
                             p_database, 
                             nullptr);

    furious::__furious_post_frame(time,
                                  p_database, 
                                  nullptr);

    p_current_app->on_frame_end();
    draw_gui();
    end_frame(p_rendering_scene);
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
