set (IMGUI_DIR ../external/imgui)
set (IMPLOT_DIR ../external/implot)
set (IMGUI_MARKDOWN_DIR ../external/imgui_markdown)
set (IMGUIZMO_DIR ../external/ImGuizmo)
set (IMNODE_DIR ../external/imnodes)
set (IMEDITOR_DIR ../external/ImGuiColorTextEdit)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMPLOT_DIR} ${IMGUI_MARKDOWN_DIR} ${IMGUIZMO_DIR} ${IMNODE_DIR} ${IMEDITOR_DIR})

add_executable(editor main.cpp)

target_link_libraries(editor cruelWindow cruelApplication cruelRender cruelGui cruelDebug ${Vulkan_LIBRARIES} ${glm_LIBRARIES})