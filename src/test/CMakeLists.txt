include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${SMILE_INCLUDE_DIR})

include_directories(${GTEST_INCLUDE_DIRS})

function(create_test NAME)
  add_executable(${NAME}
    ${NAME}.cpp
    )

  target_link_libraries(${NAME} ${GTEST_LIBRARIES} tnalib boost_context ) 
  add_test(${NAME} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/${NAME}")

  set_target_properties( ${NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/"
    )
endfunction(create_test)

SET(TESTS "tasking_test")

foreach( TEST ${TESTS} )
  create_test(${TEST})
endforeach( TEST )

add_custom_target(tests COMMAND ${CMAKE_CTEST_COMMAND}
  DEPENDS ${TESTS})

