cmake_minimum_required(VERSION 3.16.3)

set(PROJECT_NAME "synth")

project(${PROJECT_NAME})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

#include("${CMAKE_CURRENT_SOURCE_DIR}/src/backend/CMakeLists.txt")
#include("${CMAKE_CURRENT_SOURCE_DIR}/src/engine/CMakeLists.txt")

file (GLOB_RECURSE ${PROJECT_NAME}_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE ${PROJECT_NAME}_HEADERS CONFIGURE_DEPENDS "src/*.h")

find_package(openal)

add_compile_options(-O0 -g)

add_executable ( ${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_INCLUDE_DIRS})
target_link_libraries ( ${PROJECT_NAME} openal asound m SDL2)
