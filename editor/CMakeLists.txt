add_executable(editor main.cpp)



target_link_libraries(editor cruelWindow cruelApplication cruelVulkan cruelDebug ${Vulkan_LIBRARIES} ${glm_LIBRARIES})