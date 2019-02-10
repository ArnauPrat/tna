
#include "engine.h"
#include "components/mesh.h"
#include "components/transform.h"
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
  return entity;
}

void GameApp::remove_entity(Entity entity) 
{
  furious::destroy_entity(entity);
}

}
