set (IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set (IMPLOT_DIR ${CMAKE_SOURCE_DIR}/external/implot)
set (IMGUI_MARKDOWN_DIR ${CMAKE_SOURCE_DIR}/external/imgui_markdown)
set (IMGUIZMO_DIR ${CMAKE_SOURCE_DIR}/external/ImGuizmo)
set (IMNODE_DIR ${CMAKE_SOURCE_DIR}/external/imnodes)
set (IMEDITOR_DIR ${CMAKE_SOURCE_DIR}/external/ImGuiColorTextEdit)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMPLOT_DIR} ${IMGUI_MARKDOWN_DIR} ${IMGUIZMO_DIR} ${IMNODE_DIR} ${IMEDITOR_DIR})

add_executable(editor main.cpp)

target_link_libraries(editor cruelWindow cruelApplication cruelRender cruelGui cruelDebug)