
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-ignored-attributes -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 ")

### Setting up furious
set(FURIOUS_ROOT "/home/aprat/projects/personal/furious/build/dist")
set(FURIOUS_INCLUDE_DIR "${FURIOUS_ROOT}/include")
set(FURIOUS_LIBRARIES_DIR "${FURIOUS_ROOT}/lib")
#set(FURIOUS_LIBRARIES "libfurious.so")
set(FURIOUS_LIBRARIES "libfurious.so")

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
#find_package(glm REQUIRED)

### Setting up Vulkan
#find_package(Vulkan REQUIRED)
set(Vulkan_SDK_ROOT "/opt/vulkan-sdk/1.1.101.0/x86_64")
set(Vulkan_INCLUDE_DIR "${Vulkan_SDK_ROOT}/include")
set(Vulkan_LIBRARIES_DIR "${Vulkan_SDK_ROOT}/lib")
set(Vulkan_LIBRARIES "libvulkan.so")

### Setting up boost
#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#find_package(Boost 1.66 COMPONENTS program_options date_time thread system locale context REQUIRED)


include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${FURIOUS_INCLUDE_DIR})
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${Vulkan_INCLUDE_DIR})

link_directories(${BULLET_LIBRARY_DIR})
link_directories(${Vulkan_LIBRARIES_DIR})
link_directories(${FURIOUS_LIBRARIES_DIR})

message(STATUS ${FURIOUS_ROOT})
message(STATUS ${FURIOUS_INCLUDE_DIR})
message(STATUS ${FURIOUS_LIBRARIES_DIR})

set(VK_RENDERER_SOURCE_FILES 
  engine/rendering/vulkan/vkswapchain.cpp
  engine/rendering/vulkan/vkswapchain.h
  engine/rendering/vulkan/vkdevice.cpp
  engine/rendering/vulkan/vkdevice.h
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
  engine/rendering/vulkan/vkscene.h
  engine/rendering/vulkan/vkscene.cpp
  )

set(TNA_COMPONENTS
  engine/components/mesh.h
  engine/components/transform.h
  engine/components/transform_matrix.h
  )

set(TNA_SOURCE_FILES 
  ${TNA_COMPONENTS}
  ${VK_RENDERER_SOURCE_FILES}
  engine/config.cpp
  engine/config.h
  engine/engine.cpp
  engine/engine.h
  engine/error.cpp
  engine/error.h
  engine/game_app.cpp
  engine/game_app.h
  engine/log.cpp
  engine/log.h
  engine/math/math_tools.cpp
  engine/math/math_tools.h
  engine/math/matrix.h
  engine/math/vector.h
  engine/rendering/mesh_data.h
  engine/rendering/renderer.h
  engine/rendering/shader.h
  engine/rendering/vertex.h
  engine/resources/directory_registry.cpp
  engine/resources/directory_registry.h
  engine/resources/resource_registry.h
  engine/resources/resource_registry.inl
  engine/resources/resources.cpp
  engine/resources/resources.h
  engine/tools/files.cpp
  engine/tools/files.h
  engine/types.h
  engine/furious_runtime.cpp
  game.cpp
  game.h
  main.cpp
  platform.h
  tnasdk/definitions.h
	)

set(TNA_ENGINE_SCRIPTS
  engine/scripts/render_mesh.cpp
  engine/scripts/transform_hierarchy.cpp
  engine/scripts/set_camera.cpp
  )

set(TNA_SCRIPTS
  scripts/rotator.cpp
  scripts/test.cpp
  )

message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
set(CLANG_INCLUDES_DIR "/usr/lib/clang/7.0.1/include")
add_custom_command(
	OUTPUT engine/furious_runtime.cpp
	COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/engine/
	COMMAND ${FURIOUS_ROOT}/bin/fcc -o
  ${CMAKE_CURRENT_BINARY_DIR}/engine/furious_runtime.cpp ${TNA_SCRIPTS} ${TNA_ENGINE_SCRIPTS} -- --std=c++17 -I${CLANG_INCLUDES_DIR} -I${CMAKE_CURRENT_SOURCE_DIR} -I${FURIOUS_INCLUDE_DIR} -I ${Vulkan_INCLUDE_DIR}
  DEPENDS ${FURIOUS_ROOT}/bin/fcc ${TNA_SCRIPTS} ${TNA_ENGINE_SCRIPTS} ${TNA_COMPONENTS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)



add_executable(tna
	${TNA_SOURCE_FILES}
  )  

target_link_libraries(tna 
  ${FURIOUS_LIBRARIES} 
  glfw
  ${Vulkan_LIBRARIES}
  #${BULLET_LIBRARIES} 
  #  ${Boost_LIBRARIES}
  )

