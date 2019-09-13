
#include "common.h"
#include "config.h"
#include "tasking/mutex.h"
#include "game_app.h"
#include "rendering/rendering.h"
#include "resources/resources.h"
#include "gui/imgui.h"
#include "tasking/tasking.h"
#include "tasking/atomic_counter.h"
#include "data/queue.h"

#include "components/proj_view_matrix.h"
#include "components/viewport.h"
#include "components/transform.h"
#include "components/transform_matrix.h"
#include "components/render_mesh_data.h"

#include <chrono>
#include <fstream>
#include <glm/glm.hpp>
#include <furious/furious.h>


namespace tna 
{

struct task_params_t;
using task_params_queue_t = queue_t<task_params_t*>;

bool                m_show_gui = true;
bool                m_edit_mode = false;
GLFWwindow*         p_window  = nullptr;
TnaGameApp*         p_current_app = nullptr;
config_t            m_config;
furious::Database*  p_database = nullptr;;
mutex_t             m_task_params_mutex;
task_params_queue_t m_task_params_queue; 
atomic_counter_t*   m_task_counters = nullptr;
atomic_counter_t*   m_post_task_counters = nullptr;
uint32_t            m_num_threads = 1;

struct task_params_t
{
  furious::Database*    p_database = nullptr;
  float                 m_delta = 0.0;
  void*                 p_user_data = nullptr;
  furious::task_func_t  p_func = nullptr;
  uint32_t              m_queue_id = 0;
};

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

  // Initializing resource directory registry
  directory_registry_init();
  directory_registry_insert("./");

  // Initializing tasking's system thread pool
  mutex_init(&m_task_params_mutex);
  queue_init(&m_task_params_queue, 128);
  m_num_threads = 1;
  start_thread_pool(m_num_threads);

  // Reading engine's config
  if(file_exists("./config.ini")) 
  {
    config_init(&m_config, "./config.ini");
  } 

  resources_init();

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

  resources_release();
  config_release(&m_config);
  stop_thread_pool();
  task_params_t* next = nullptr;
  while(queue_pop(&m_task_params_queue, &next))
  {
    delete next;
  }
  queue_release(&m_task_params_queue);
  mutex_release(&m_task_params_mutex);
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


static void
run_task(const furious::task_t* furious_task,
         float delta,
         furious::Database* database,
         void* user_data,
         atomic_counter_t* sync_counter)
{

  task_params_t* tmp_task_params[m_num_threads];
  for(uint32_t i = 0; i < m_num_threads; ++i)
  {
    task_params_t* params = nullptr;
    queue_pop(&m_task_params_queue, &params);
    if(params == nullptr)
    {
      params = new task_params_t();
    }
    tmp_task_params[i] = params;
    params->m_delta = delta;
    params->p_database = database;
    params->p_user_data = user_data;
    params->p_func = furious_task->p_func;
    params->m_queue_id = i;
    task_t task;
    task.p_fp=[] (void* t) {
      task_params_t* params = (task_params_t*)t;
      params->p_func(params->m_delta, 
                     params->p_database,
                     params->p_user_data,
                     1,
                     params->m_queue_id,
                     m_num_threads); 
    };
    task.p_args=params;
    execute_task_async(i, task, sync_counter);
  }
  atomic_counter_join(sync_counter);
  for(uint32_t i = 0; i < m_num_threads; ++i)
  {
    queue_push(&m_task_params_queue, tmp_task_params[i]);
  }
}

static bool
all_visited_parents(const furious::task_graph_t* task_graph, 
                    uint32_t node_id,
                    bool * visited)
{
  const DynArray<uint32_t> parents = task_graph->m_tasks[node_id].m_parents;
  const uint32_t num_elements = parents.size();
  for(uint32_t i = 0; i < num_elements; ++i)
  {
    if(!visited[parents[i]])
    {
      return false;
    }
  }
  return true;
}

static void
run_furious_task_graph(const furious::task_graph_t* task_graph, 
                       atomic_counter_t* task_counters,
                       float delta, 
                       furious::Database* database, 
                       void* user_data)
{
  const uint32_t num_nodes = task_graph->m_num_tasks;
  bool visited_nodes[num_nodes];
  memset(visited_nodes, 0, sizeof(bool)*num_nodes);
  bool found = true;
  while(found)
  {
    found = false;
    for (uint32_t i = 0; i < num_nodes; ++i) 
    {
      if(!visited_nodes[i] && all_visited_parents(task_graph, i, visited_nodes))
      {
        found = true;
        DynArray<uint32_t> next_frontier;
        DynArray<uint32_t> current_frontier;
        current_frontier.append(i);
        visited_nodes[i] = true;
        while(current_frontier.size() > 0)
        {
          const uint32_t frontier_size = current_frontier.size();
          for(uint32_t ii = 0; ii < frontier_size; ++ii)
          {
            const uint32_t next_node_id = current_frontier[ii];

            run_task(&task_graph->m_tasks[next_node_id],
                     delta,
                     database,
                     user_data,
                     &task_counters[next_node_id]);

            const furious::task_t* next_node = &task_graph->m_tasks[next_node_id]; 
            const DynArray<uint32_t>& children = next_node->m_children;
            const uint32_t num_children = children.size();
            for(uint32_t j = 0; j < num_children; ++j)
            {
              const uint32_t child_id = children[j];
              if(!visited_nodes[child_id] && all_visited_parents(task_graph, child_id, visited_nodes))
              {
                next_frontier.append(child_id);
                visited_nodes[child_id] = true;
              }
            }
          }
          current_frontier = next_frontier;
          next_frontier.clear();
        }
      }
    }
  }

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

  furious::task_graph_t* task_graph = furious::__furious_task_graph();
  furious::task_graph_t* post_task_graph = furious::__furious_post_task_graph();
  m_task_counters = new atomic_counter_t[task_graph->m_num_tasks];
  m_post_task_counters = new atomic_counter_t[post_task_graph->m_num_tasks];

  for (uint32_t i = 0; i < task_graph->m_num_tasks; ++i) 
  {
    atomic_counter_init(&m_task_counters[i]);
  }
  for (uint32_t i = 0; i < post_task_graph->m_num_tasks; ++i) 
  {
    atomic_counter_init(&m_post_task_counters[i]);
  }

  while (!glfwWindowShouldClose(p_window)) 
  {
    // Keep running
    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
    start_time = current_time;

    glfwPollEvents();
    begin_frame(p_rendering_scene);
    p_current_app->on_frame_start(time);
    run_furious_task_graph(task_graph, m_task_counters, time, p_database, nullptr);
    for(uint32_t i = 0; i < task_graph->m_num_tasks; ++i)
    {
      atomic_counter_join(&m_task_counters[i]);
    }

    run_furious_task_graph(post_task_graph, m_post_task_counters, time, p_database, nullptr);
    for(uint32_t i = 0; i < post_task_graph->m_num_tasks; ++i)
    {
      atomic_counter_join(&m_post_task_counters[i]);
    }

    /*furious::__furious_frame(time, 
                             p_database, 
                             nullptr);

    furious::__furious_post_frame(time,
                                  p_database, 
                                  nullptr);
                                  */

    p_current_app->on_frame_end();
    draw_gui();
    end_frame(p_rendering_scene);
  }

  for (uint32_t i = 0; i < task_graph->m_num_tasks; ++i) 
  {
    atomic_counter_release(&m_task_counters[i]);
  }
  for (uint32_t i = 0; i < post_task_graph->m_num_tasks; ++i) 
  {
    atomic_counter_release(&m_post_task_counters[i]);
  }
  delete [] m_task_counters;
  delete [] m_post_task_counters;

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
