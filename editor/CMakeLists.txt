add_executable(editor main.cpp)

target_link_libraries(editor cruelWindow cruelApplication cruelRender cruelGui cruelDebug ${Vulkan_LIBRARIES} ${glm_LIBRARIES})