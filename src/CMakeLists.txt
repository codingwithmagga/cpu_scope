add_library(cpuscope_lib STATIC
  cpuscope_lib.cpp
  cpuscope_lib.hpp
)

target_include_directories(cpuscope_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(cpuscope_lib PUBLIC Threads::Threads)

if(LLVM_FOUND)
  message(STATUS "Optional LLVM integration enabled: ${LLVM_PACKAGE_VERSION}")
  target_link_libraries(cpuscope_lib PUBLIC LLVM)
else()
  message(STATUS "LLVM not found; continuing without optional LLVM support.")
endif()

target_compile_features(cpuscope_lib PUBLIC cxx_std_20)

add_executable(cpuscope_cli main.cpp)

target_link_libraries(cpuscope_cli PRIVATE cpuscope_lib)

set_target_properties(cpuscope_cli PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
