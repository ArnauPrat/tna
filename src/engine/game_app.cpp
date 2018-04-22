
#include "engine.h"
#include <btBulletDynamicsCommon.h>
#include <furious/furious.h>

namespace tna { 

GameApp::GameApp() {

  // Initializing Bullet Physics
  int32_t max_objects = 4096;
  int32_t width = 1024;
  int32_t height = 1024;
  btVector3 worldAabbMin(-width/2, -height/2, -1);
  btVector3 worldAabbMax(width/2, height/2, 1);
  p_bt_configuration = new btDefaultCollisionConfiguration();
  p_bt_dispatcher    = new btCollisionDispatcher(p_bt_configuration);
  p_bt_broadphase    = new bt32BitAxisSweep3(worldAabbMin, worldAabbMax, max_objects, 0, true);
  p_bt_world         = new btCollisionWorld(p_bt_dispatcher, p_bt_broadphase, p_bt_configuration); 

  p_database = furious::create_database();
  p_workload = furious::create_workload();

}

GameApp::~GameApp() {

  furious::destroy_workload(p_workload);
  furious::destroy_database(p_database);

  // Stopping Bullet Physics
  delete p_bt_world;
  delete p_bt_broadphase;
  delete p_bt_dispatcher;
  delete p_bt_configuration;
}

Entity GameApp::create_entity() {
  return Entity::create_entity(p_database);
}

void GameApp::remove_entity(Entity* entity) {
  Entity::remove_entity(entity);
}

}
