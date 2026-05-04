add_executable(cpuscope_tests test_main.cpp)

target_link_libraries(cpuscope_tests PRIVATE cpuscope_lib)

set_target_properties(cpuscope_tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(NAME cpuscope_tests COMMAND cpuscope_tests)
