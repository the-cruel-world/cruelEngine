file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelOpenGL SHARED ${SRC})

target_link_libraries(cruelOpenGL glfw ${glm_LIBRARIES})