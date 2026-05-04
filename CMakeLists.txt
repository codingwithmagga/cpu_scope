cmake_minimum_required(VERSION 3.25)

project(cpuscope VERSION 0.1.0 LANGUAGES CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds are not allowed. Please use a separate build directory.")
endif()

option(CPUSCOPE_WERROR "Enable CI build mode and treat warnings as errors" OFF)
option(ENABLE_MARCH_NATIVE "Enable -march=native optimization" OFF)
option(CPUSCOPE_BUILD_TESTS "Build tests" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
  add_compile_options(-Wall -Wextra -Wpedantic)
  if(CPUSCOPE_WERROR)
    add_compile_options(-Werror)
  endif()
  if(ENABLE_MARCH_NATIVE)
    add_compile_options(-march=native)
  endif()
endif()

if(NOT CMAKE_CXX_FLAGS MATCHES "-O[0-3s]")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

find_package(Threads REQUIRED)
find_package(LLVM QUIET)

if(CPUSCOPE_BUILD_TESTS)
  include(CTest)
  enable_testing()

  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
  )
  FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(lib)
add_subdirectory(cli)
if(CPUSCOPE_BUILD_TESTS)
  add_subdirectory(tests)
endif()
