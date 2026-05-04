add_library(cpuscope_lib STATIC
  src/cpuscope_lib.cpp
)

target_include_directories(cpuscope_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(cpuscope_lib PUBLIC Threads::Threads)

if(LLVM_FOUND)
  message(STATUS "Optional LLVM integration enabled: ${LLVM_PACKAGE_VERSION}")
  target_link_libraries(cpuscope_lib PUBLIC LLVM)
else()
  message(STATUS "LLVM not found; continuing without optional LLVM support.")
endif()

target_compile_features(cpuscope_lib PUBLIC cxx_std_20)