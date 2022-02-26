cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mc-clone)

find_package(GLEW 2.2 REQUIRED)

include(FindPNG)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glfw)

add_subdirectory(deps/glm)

if(NOT APP_RESOURCE_PATH)
  set(APP_RESOURCE_PATH "..")
endif()
configure_file(build_config.h.in build_config.h)

file(GLOB SOURCE_FILES *.cpp)

add_executable(mc-clone ${SOURCE_FILES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(mc-clone GLEW glfw GL glm::glm PNG::PNG)
