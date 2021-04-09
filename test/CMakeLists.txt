set (IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set (IMPLOT_DIR ${CMAKE_SOURCE_DIR}/external/implot)
set (IMGUI_MARKDOWN_DIR ${CMAKE_SOURCE_DIR}/external/imgui_markdown)
set (IMGUIZMO_DIR ${CMAKE_SOURCE_DIR}/external/ImGuizmo)
set (IMNODE_DIR ${CMAKE_SOURCE_DIR}/external/imnodes)
set (IMEDITOR_DIR ${CMAKE_SOURCE_DIR}/external/ImGuiColorTextEdit)
set (TINYGLTF_DIR ${CMAKE_SOURCE_DIR}/external/tinygltf)

include_directories(${IMGUI_DIR} ${TINYGLTF_DIR} ${IMGUI_DIR}/backends ${IMPLOT_DIR} ${IMGUI_MARKDOWN_DIR} ${IMGUIZMO_DIR} ${IMNODE_DIR} ${IMEDITOR_DIR})

add_executable(core core.cpp)

target_link_libraries(core cruelCore)

add_executable(main main.cpp)

target_link_libraries(main cruelWindow cruelApplication cruelDebug cruelGui cruelScene cruelRender)

add_executable(render render.cpp)

target_link_libraries(render cruelWindow cruelApplication cruelDebug cruelGui cruelScene cruelRender ${Vulkan_LIBRARIES} ${glm_LIBRARIES})

add_executable(gltf gltf.cpp)

target_precompile_headers(gltf PUBLIC "${TINYGLTF_DIR}/tiny_gltf.h" "${TINYGLTF_DIR}/json.hpp")

target_link_libraries(gltf)