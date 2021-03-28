file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

set (IMGUI_DIR ../../external/imgui)

set (IMGUI_SRC 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp 
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp
)

include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

add_library(cruelGui SHARED ${SRC} ${IMGUI_SRC})

target_link_libraries(cruelGui cruelRender)