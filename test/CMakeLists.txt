add_executable(main main.cpp)

target_link_libraries(main)

add_executable(render render.cpp)

target_link_libraries(render cruelWindow cruelApplication cruelDebug cruelScene cruelVulkan ${Vulkan_LIBRARIES} ${glm_LIBRARIES})