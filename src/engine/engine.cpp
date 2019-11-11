
#include "common.h"
#include "config.h"
#include "tasking/mutex.h"
#include "game_app.h"
#include "rendering/rendering.h"
#include "resources/resources.h"
#include "gui/imgui.h"
#include "trace/trace.h"
#include "tasking/tasking.h"
#include "tasking/atomic_counter.h"
#include "tasking/barrier.h"
#include "data/queue.h"
#include "gui/widgets/tasking_widget.h"

#include "components/proj_view_matrix.h"
#include "components/viewport.h"
#include "components/transform.h"
#include "components/transform_matrix.h"
#include "components/render_mesh_data.h"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <glm/glm.hpp>
#include <furious/furious.h>


namespace tna 
{

struct task_params_t;
using task_params_queue_t = queue_t<task_params_t*>;

static bool                m_show_gui = true;
static bool                m_edit_mode = false;
static GLFWwindow*         p_window  = nullptr;
static TnaGameApp*         p_current_app = nullptr;
static config_t            m_config;
static furious::Database*  p_database = nullptr;;
static mutex_t             m_task_params_mutex;
static task_params_queue_t m_task_params_queue; 
static atomic_counter_t*   m_task_counters = nullptr;
static atomic_counter_t*   m_post_task_counters = nullptr;
static float               m_last_game_loop_time = 0.0f;
static bool                m_ready = false;

static std::condition_variable*     m_main_thread_cond = nullptr;
static std::mutex*                  m_main_thread_mutex = nullptr;

/**
 * \brief Structure to pass parameters to takss
 */
struct task_params_t
{
  furious::Database*    p_database = nullptr;   //< pointer to database
  float                 m_delta = 0.0;          //< delta from last frame
  void*                 p_user_data = nullptr;  //< pointer to user data
  furious::task_func_t  p_func = nullptr;       //< task function to execute
  uint32_t              m_queue_id = 0;         //< the queue to add the task to
  barrier_t*            p_barrier = nullptr;    //< the barrier to synchronize parallel tasks (null if not needed)
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

void
initialize() 
{
  // Initializing loging system
  log_init("./logfile.log");

  // Initializing resource directory registry
  directory_registry_init();
  directory_registry_insert("./");

  // Reading engine's config
  config_init(&m_config, "./config.ini");

  // Initializing tasking's system thread pool
  mutex_init(&m_task_params_mutex);
  queue_init(&m_task_params_queue, 128);
  trace_init(m_config.m_num_worker_threads+1);
  tasking_start_thread_pool(m_config.m_num_worker_threads);

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
    TNA_LOG_ERROR(TNA_ERROR::E_RENDERER_INITIALIZATION_ERROR, 
                  "Error creating window");
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
  tasking_stop_thread_pool();
  trace_release();
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
    //bool aux = true; ImGui::ShowDemoWindow(&aux);

    ImGui::Begin("TNA");                         
    ImGui::Text("Frame time %.3f ms",
                1000.0f / ImGui::GetIO().Framerate);

    ImGui::Text("FPS %.1f", 
                ImGui::GetIO().Framerate);

    ImGui::Text("Game Loop time: %.5f", 
                m_last_game_loop_time);
    ImGui::End();

    tasking_widget_render();

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

  task_params_t* tmp_task_params[m_config.m_num_worker_threads];
  barrier_t barrier;
  barrier_init(&barrier);
  for(uint32_t i = 0; i < m_config.m_num_worker_threads; ++i)
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
    params->p_barrier = &barrier;
    task_t task;
    task.p_fp=[] (void* t) {
      task_params_t* params = (task_params_t*)t;
      params->p_func(params->m_delta, 
                     params->p_database,
                     params->p_user_data,
                     16,
                     params->m_queue_id,
                     m_config.m_num_worker_threads,
                     params->p_barrier); 
    };
    task.p_args=params;

    char name[_TNA_TASKING_MAX_NAME_STRING_LENGTH];
    snprintf(name, 
             _TNA_TASKING_MAX_NAME_STRING_LENGTH, 
             "Task %d", 
             furious_task->m_id);

    char info[_TNA_TASKING_MAX_INFO_STRING_LENGTH];
    snprintf(info, 
             _TNA_TASKING_MAX_INFO_STRING_LENGTH, 
             "Task %d block_size: %d, worker: %d, num_threads: %d \n %s", 
             furious_task->m_id, 
             16, 
             i, 
             m_config.m_num_worker_threads, 
             furious_task->p_info);

    tasking_execute_task_async(i, task, sync_counter, name, info);
  }
  tasking_yield(sync_counter);
  //atomic_counter_join(sync_counter);
  barrier_release(&barrier);
  for(uint32_t i = 0; i < m_config.m_num_worker_threads; ++i)
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


void update_game_logic(void* ptr)
{
  furious::task_graph_t* task_graph = furious::__furious_task_graph();
  furious::task_graph_t* post_task_graph = furious::__furious_post_task_graph();
  run_furious_task_graph(task_graph, m_task_counters, *(float*)ptr, p_database, nullptr);
  for(uint32_t i = 0; i < task_graph->m_num_tasks; ++i)
  {
    atomic_counter_join(&m_task_counters[i]);
  }

  run_furious_task_graph(post_task_graph, m_post_task_counters, *(float*)ptr, p_database, nullptr);
  for(uint32_t i = 0; i < post_task_graph->m_num_tasks; ++i)
  {
    atomic_counter_join(&m_post_task_counters[i]);
  }

  {
    std::unique_lock<std::mutex> lock(*m_main_thread_mutex);
    m_ready = true;
    m_main_thread_cond->notify_one();
  }
}

void
run(TnaGameApp* game_app) 
{

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

  static auto start_time = std::chrono::high_resolution_clock::now();
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

  m_main_thread_mutex   = new std::mutex();
  m_main_thread_cond    = new std::condition_variable();

  while (!glfwWindowShouldClose(p_window)) 
  {
    TRACE_RECORD(m_config.m_num_worker_threads, 
                 trace_event_type_t::E_NEW_FRAME, 
                 nullptr,
                 nullptr);
    
    // Keep running
    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
    start_time = current_time;

    glfwPollEvents();
    begin_frame(p_rendering_scene);
    p_current_app->on_frame_start(time);
    auto game_loop_start = std::chrono::high_resolution_clock::now();

    {
      std::unique_lock<std::mutex> lock(*m_main_thread_mutex);
      m_ready = false;
      task_t task;
      task.p_args = &time;
      task.p_fp = update_game_logic;
      //atomic_counter_t sync_counter;
      //atomic_counter_init(&sync_counter);
      tasking_execute_task_async(0, 
                                 task, 
                                 nullptr,
                                 "update_game_logic", 
                                 "update_game_logic");
      //atomic_counter_join(&sync_counter);
      //atomic_counter_release(&sync_counter);

      m_main_thread_cond->wait(lock, 
                               [&] () {
                               return m_ready;
                               }
                              );
    }

    auto game_loop_end = std::chrono::high_resolution_clock::now();
    m_last_game_loop_time = std::chrono::duration<float, std::chrono::milliseconds::period>(game_loop_end - game_loop_start).count();

    p_current_app->on_frame_end();

    TRACE_RECORD(m_config.m_num_worker_threads, 
                 trace_event_type_t::E_TASK_START,
                 "Rendering", 
                 "");
    draw_gui();
    end_frame(p_rendering_scene);

    TRACE_RECORD(m_config.m_num_worker_threads, 
                 trace_event_type_t::E_TASK_STOP,
                 "Rendering", 
                 "");
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


  delete m_main_thread_mutex;
  delete m_main_thread_cond;
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
