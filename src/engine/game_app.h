


#ifndef _GAME_APP_H_
#define _GAME_APP_H_ value

#include "entity.h"

#include <furious/furious.h>

struct GLFWwindow;

class btCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btCollisionWorld;

namespace tna {

class GameApp {
public:

  GameApp();
  virtual ~GameApp();

  virtual void on_app_start() = 0;

  virtual void on_app_finish() = 0;

  virtual void on_frame_update() = 0;

  virtual void on_key_event(GLFWwindow* window, 
                            int key, 
                            int scancode, 
                            int action, 
                            int mods) = 0;

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  

  /**
   * @brief Creates an entity in the this game app
   *
   * @return The newly created entity
   */
  Entity create_entity();

  /**
   * @brief Removes an entity in this current game app
   *
   * @param entity The entity to obe removed
   */
  void remove_entity(Entity* entity);

protected:

  // BULLET PHYSICS
  btCollisionConfiguration* p_bt_configuration;
  btCollisionDispatcher*    p_bt_dispatcher;
  btBroadphaseInterface*    p_bt_broadphase;
  btCollisionWorld*         p_bt_world;

  // FURIOUS
  furious::Database*        p_database;
  furious::Workload*        p_workload;
};

}

#endif /* ifndef _GAME_APP_H_ */
