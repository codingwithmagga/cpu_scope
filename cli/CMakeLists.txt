add_executable(cpuscope_cli main.cpp)

target_link_libraries(cpuscope_cli PRIVATE cpuscope_lib)

set_target_properties(cpuscope_cli PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)