add_executable(main main.cpp)

target_link_libraries(main realWindow realApplication realRender realDebug glfw ${Vulkan_LIBRARIES} ${glm_LIBRARIES})