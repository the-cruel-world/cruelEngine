add_executable(triangle triangle.cpp)
target_link_libraries(triangle cruelWindow cruelApplication cruelVulkan cruelDebug ${Vulkan_LIBRARIES} ${glm_LIBRARIES})

add_executable(draw_index draw_index.cpp)
target_link_libraries(draw_index cruelWindow cruelApplication cruelVulkan cruelDebug ${Vulkan_LIBRARIES} ${glm_LIBRARIES})