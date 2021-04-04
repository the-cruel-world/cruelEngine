set (IMGUI_DIR ../external/imgui)
set (IMPLOT_DIR ../external/implot)
set (IMGUI_MARKDOWN_DIR ../external/imgui_markdown)
set (IMGUIZMO_DIR ../external/ImGuizmo)
set (IMNODE_DIR ../external/imnodes)
set (IMEDITOR_DIR ../external/ImGuiColorTextEdit)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMPLOT_DIR} ${IMGUI_MARKDOWN_DIR} ${IMGUIZMO_DIR} ${IMNODE_DIR} ${IMEDITOR_DIR})

add_executable(core core.cpp)

target_link_libraries(core cruelCore)

add_executable(main main.cpp)

target_link_libraries(main cruelWindow cruelApplication cruelDebug cruelGui cruelScene cruelRender)

add_executable(render render.cpp)

target_link_libraries(render cruelWindow cruelApplication cruelDebug cruelGui cruelScene cruelRender ${Vulkan_LIBRARIES} ${glm_LIBRARIES})