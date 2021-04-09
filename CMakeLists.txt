cmake_minimum_required(VERSION 3.8.0)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cruelEngine)

set(default_build_type "Release")

OPTION(DEBUG "Enable debug for all" OFF)
OPTION(RENDER_DEBUG "Enbale debug for render" OFF)

string(ASCII 27 ESCAPE)
set (MESSAGE_HEADER "${ESCAPE}[31m[Note]:${ESCAPE}[0m ")

IF(enable_DEBUG)
    set(enable_RENDER_DEBUG true)
    message(${MESSAGE_HEADER} "enable debug")
    set (CMAKE_BUILD_TYPE Debug)
ENDIF(enable_DEBUG)

IF(enable_RENDER_DEBUG)
    add_compile_definitions(RENDER_DEBUG=1)
    message(${MESSAGE_HEADER} "enable render debug")
ENDIF(enable_RENDER_DEBUG)

IF (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release)
    message(${MESSAGE_HEADER} "Build type: " ${CMAKE_BUILD_TYPE})
ENDIF ()

OPTION(DYNAMIC "Library build type" ON)
IF(DYNAMIC)
set (LIBRARY_TYPE SHARED)
else (DYNAMIC)
set (LIBRARY_TYPE STATIC)
ENDIF(DYNAMIC)

FIND_PACKAGE(PkgConfig REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED FATAL_ERROR)

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${PROJECT_INCLUDE_DIR}
    ${glfw3_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS})

# build the library
add_subdirectory(src)

# add_subdirectory(examples)

OPTION(EDITOR "Build CruelEngine Editor" ON)
if (EDITOR)
# build the editor
add_subdirectory(editor)
endif(EDITOR)

OPTION(TEST "Build CruelEngine tests" ON)
if (TEST)
# build test
add_subdirectory(test)
endif(TEST)