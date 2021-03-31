file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

set (IMGUI_DIR ../../external/imgui)
set (IMPLOT_DIR ../../external/implot)
set (IMGUI_MARKDOWN_DIR ../../external/imgui_markdown)
set (IMGUIZMO_DIR ../../external/ImGuizmo)
set (IMNODE_DIR ../../external/imnodes)
set (IMEDITOR_DIR ../../external/ImGuiColorTextEdit)


set (IMGUI_SRC 
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp 
    ${IMPLOT_DIR}/implot_demo.cpp
    ${IMEDITOR_DIR}/TextEditor.cpp
)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMPLOT_DIR} ${IMGUI_MARKDOWN_DIR} ${IMGUIZMO_DIR} ${IMNODE_DIR} ${IMEDITOR_DIR})

add_library(cruelGui SHARED ${IMGUI_SRC} ${SRC})

# target_compile_definitions(cruelGui PRIVATE -DImDrawIdx)
target_compile_definitions(cruelGui PRIVATE ImDrawIdx=unsigned\ int)

target_link_libraries(cruelGui cruelRender)