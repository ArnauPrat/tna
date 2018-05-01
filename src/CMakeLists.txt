
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wno-ignored-attributes -fexceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -O0 -g -pg ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 ")

### Setting up furious
set(FURIOUS_ROOT "/home/aprat/projects/personal/furious/build/dist")
set(FURIOUS_INCLUDE_DIR "${FURIOUS_ROOT}/include")
set(FURIOUS_LIB_DIR "${FURIOUS_ROOT}/lib")
set(FURIOUS_LIBS libfurious.so) ### Setting up bullet
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

### Setting up Vulkan
#find_package(Vulkan REQUIRED)
set(Vulkan_SDK_ROOT "/usr/local/vulkan-sdk/x86_64")
set(Vulkan_INCLUDE_DIR "${Vulkan_SDK_ROOT}/include")
set(Vulkan_LIBRARIES_DIR "${Vulkan_SDK_ROOT}/lib")
set(Vulkan_LIBRARIES "libvulkan.so")

### Setting up boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.66 COMPONENTS program_options date_time thread system locale context REQUIRED)


include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${FURIOUS_INCLUDE_DIR})
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${Vulkan_INCLUDE_DIR})

link_directories(${FURIOUS_LIB_DIR})
link_directories(${BULLET_LIBRARY_DIR})
link_directories(${Vulkan_LIBRARIES_DIR})

message(STATUS ${FURIOUS_ROOT})
message(STATUS ${FURIOUS_INCLUDE_DIR})
message(STATUS ${FURIOUS_LIB_DIR})

set(VK_RENDERER_SOURCE_FILES 
  engine/rendering/vulkan/vkbuffer_tools.cpp
  engine/rendering/vulkan/vkbuffer_tools.h
  engine/rendering/vulkan/vkmesh_data.cpp
  engine/rendering/vulkan/vkmesh_data.h
  engine/rendering/vulkan/vkrenderer.cpp
  engine/rendering/vulkan/vkrenderer.h
  engine/rendering/vulkan/vkshader.cpp
  engine/rendering/vulkan/vkshader.h
  engine/rendering/vulkan/vkshader_tools.cpp
  engine/rendering/vulkan/vkshader_tools.h
  engine/rendering/vulkan/vkvertex_tools.cpp
  engine/rendering/vulkan/vkvertex_tools.h
  engine/rendering/vulkan/vkmem_alloc.h
  )

set(TNA_SOURCE_FILES 
  ${VK_RENDERER_SOURCE_FILES}
  engine/config.cpp
  engine/config.h
  engine/engine.cpp
  engine/engine.h
  engine/game_app.cpp
  engine/game_app.h
  engine/log.cpp
  engine/log.h
  engine/rendering/mesh_data.h
  engine/rendering/renderer.h
  engine/rendering/shader.h
  engine/rendering/vertex.h
  engine/resources/directory_registry.cpp
  engine/resources/directory_registry.h
  engine/resources/resource_registry.h
  engine/resources/resource_registry.inl
  engine/tools/files.cpp
  engine/tools/files.h
  engine/types.h
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
  ${Vulkan_LIBRARIES}
  ${Boost_LIBRARIES}
  )

