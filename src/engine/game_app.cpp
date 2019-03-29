
#include "engine.h"
#include "components/mesh.h"
#include "components/transform.h"
#include "components/transform_matrix.h"
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
  m_camera = create_entity();
  m_camera.add_tag("MainCamera");

  Transform* transform = m_camera.get_component<Transform>();
  transform->m_position = {0.0,2.5,-10.0};
  transform->m_local_rotation = {0.0,180.0,0.0};

}

}
