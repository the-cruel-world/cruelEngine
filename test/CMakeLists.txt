set (IMGUI_DIR ../external/imgui)
set (IMPLOT_DIR ../external/implot)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMPLOT_DIR})

add_executable(main main.cpp)

target_link_libraries(main cruelWindow cruelScene)

add_executable(render render.cpp)

target_link_libraries(render cruelWindow cruelApplication cruelDebug cruelGui cruelScene cruelRender ${Vulkan_LIBRARIES} ${glm_LIBRARIES})