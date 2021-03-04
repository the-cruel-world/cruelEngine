cmake_minimum_required(VERSION 3.8.0)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-Wall -O2" )
set(CMAKE_CXX_STANDARD 17)

project(cruelEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FIND_PACKAGE(PkgConfig REQUIRED)

FIND_PACKAGE(glfw3 REQUIRED FATAL_ERROR)
FIND_PACKAGE(glm REQUIRED FATAL_ERROR)

include_directories(src
    external/stb_image/
    ${glfw3_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS})

add_subdirectory(src)
#add_subdirectory(examples)
#add_subdirectory(editor)
#add_subdirectory(test)