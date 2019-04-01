
#include "engine.h"
#include "components/mesh.h"
#include "components/transform.h"
#include "components/transform_matrix.h"
#include "components/look_at_camera.h"
#include "components/fps_camera.h"
#include "math/matrix.h"
#include "math/math_tools.h"
#include <furious/furious.h>

namespace tna 
{ 

extern furious::Database* p_database;

GameApp::GameApp() 
{


}

GameApp::~GameApp() 
{

}

Entity 
GameApp::create_entity() 
{
  Entity entity = furious::create_entity(p_database);
  FURIOUS_ADD_COMPONENT((&entity),Transform);
  FURIOUS_ADD_COMPONENT((&entity),TransformMatrix);
  return entity;
}

void GameApp::remove_entity(Entity entity) 
{
  furious::destroy_entity(entity);
}

void
GameApp::start()
{
  /*m_camera = furious::create_entity(p_database);
  FURIOUS_ADD_COMPONENT((&m_camera),
                        LookAtCamera,
                        Vector3(0.0f,1.0f,10.0f),
                        Vector3(0.0f,0.0f, 0.0f),
                        Vector3(0.0f, 1.0f, 0.0f));
                        */
  m_camera = furious::create_entity(p_database);
  FURIOUS_ADD_COMPONENT((&m_camera),
                        FPSCamera,
                        Vector3(0.0f,0.0f,10.0f),
                        radians(0.0),
                        radians(0.0));
  m_camera.get_component<FPSCamera>()->m_speed = 3.0f;
}

}
