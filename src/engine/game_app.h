


#ifndef _TNA_GAME_APP_H_
#define _TNA_GAME_APP_H_ 

#include "common.h"
#include "entity.h"

struct GLFWwindow;

namespace tna 
{

class GameApp 
{
public:

  GameApp();
  virtual ~GameApp();

  void
  start();

  virtual void 
  on_app_start() = 0;

  virtual void 
  on_app_finish() = 0;

  virtual void 
  on_frame_update(float delta) = 0;

  virtual void 
  on_key_event(GLFWwindow* window, 
               int32_t key, 
               int32_t scancode, 
               int32_t action, 
               int32_t mods) = 0;

  virtual void 
  on_cursor_position(GLFWwindow* window,
                     double xpos,
                     double ypos) = 0;

  virtual void 
  on_mouse_button(GLFWwindow* window, 
                  int32_t button, 
                  int32_t action, 
                  int32_t mods) = 0;

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  

  /**
   * \brief Creates an entity in the this game app
   *
   * \return The newly created entity
   */
  Entity 
  create_entity();

  /**
   * \brief Removes an entity in this current game app
   *
   * \param entity The entity to be removed
   */
  void 
  remove_entity(Entity entity);

  /**
   * \brief Creates the camera for the scene.
   */
  void
  create_camera();

protected:
  Entity m_camera;
};

}

#endif /* ifndef _GAME_APP_H_ */
