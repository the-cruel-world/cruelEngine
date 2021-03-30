file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

set (IMGUI_DIR ../../external/imgui)
set (IMPLOT_DIR ../../external/implot)

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
)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMPLOT_DIR})

add_library(cruelGui SHARED ${IMGUI_SRC} ${SRC})

target_link_libraries(cruelGui cruelRender)