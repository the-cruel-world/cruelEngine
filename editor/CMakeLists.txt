add_executable(editor main.cpp)

target_link_libraries(editor realWindow realApplication realRender realDebug glfw ${Vulkan_LIBRARIES} ${glm_LIBRARIES})