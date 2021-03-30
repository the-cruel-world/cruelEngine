set (IMGUI_DIR ../external/imgui)
set (IMPLOT_DIR ../external/implot)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMPLOT_DIR})

add_executable(editor main.cpp)

target_link_libraries(editor cruelWindow cruelApplication cruelRender cruelGui cruelDebug ${Vulkan_LIBRARIES} ${glm_LIBRARIES})