file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(realRender SHARED ${SRC})

target_link_libraries(realRender glfw ${Vulkan_LIBRARIES} ${glm_LIBRARIES})