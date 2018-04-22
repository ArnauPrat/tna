
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wno-ignored-attributes")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -O0 -g -pg ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG ")

### Setting up furious
set(FURIOUS_ROOT "/home/aprat/projects/personal/furious/build/dist")
set(FURIOUS_INCLUDE_DIR "${FURIOUS_ROOT}/include")
set(FURIOUS_LIB_DIR "${FURIOUS_ROOT}/lib")
set(FURIOUS_LIBS libfurious.so)

### Setting up bullet
set(BULLET_ROOT "/usr/")
set(BULLET_INCLUDE_DIR "${BULLET_ROOT}/include/bullet")
set(BULLET_LIBRARY_DIR "${BULLET_ROOT}/lib")
set(BULLET_LIBRARIES
	libBullet3Common.so
	libBulletCollision.so
	libBullet3Collision.so
	libBulletDynamics.so
	libBullet3Dynamics.so
	libBullet3Geometry.so
	libLinearMath.so
	) 

### Setting up bgfx
find_package(glfw3 3.2 REQUIRED)

### Setting up GLM
find_package(glm REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${FURIOUS_INCLUDE_DIR})
include_directories(${BULLET_INCLUDE_DIR})

link_directories(${FURIOUS_LIB_DIR})
link_directories(${BULLET_LIBRARY_DIR})

message(STATUS ${FURIOUS_ROOT})
message(STATUS ${FURIOUS_INCLUDE_DIR})
message(STATUS ${FURIOUS_LIB_DIR})

set(TNA_SOURCE_FILES 
  engine/config.cpp
  engine/config.h
  engine/engine.cpp
  engine/engine.h
  engine/log.cpp
  engine/log.h
  engine/types.h
  engine/game_app.h
  engine/game_app.cpp
  game.cpp
  game.h
  main.cpp
  platform.h
  tnasdk/definitions.h
	)

add_executable(tna
	${TNA_SOURCE_FILES})  

target_link_libraries(tna 
  ${FURIOUS_LIBS} 
  ${BULLET_LIBRARIES} 
  glfw
  )

