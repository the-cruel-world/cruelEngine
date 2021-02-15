add_executable(main main.cpp)

target_link_libraries(main cruelWindow ${Vulkan_LIBRARIES} ${glm_LIBRARIES})