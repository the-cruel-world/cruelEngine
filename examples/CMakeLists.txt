


add_executable(triangle triangle.cpp)
target_link_libraries(triangle cruelWindow cruelApplication cruelVulkan cruelDebug ${Vulkan_LIBRARIES} ${glm_LIBRARIES})