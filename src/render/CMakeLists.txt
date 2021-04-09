FIND_PACKAGE(Vulkan REQUIRED FATAL_ERROR)

file (GLOB VULKAN_CORE_SRC "vulkan/*.cpp" "vulkan/*.hpp" "vulkan/*.h")
file (GLOB VULKAN_CORE_HEADERS "vulkan/*.hpp" "vulkan/*.h")

file (GLOB RENDER_TASKS "render_tasks/*.cpp" "render_tasks/*.hpp" "render_tasks/*.h")
file (GLOB RENDER_TASKS_HEADERS "render_tasks/*.hpp" "render_tasks/*.h")

file (GLOB RENDER_SRC "*.cpp" "*.hpp" "*.h" )
file (GLOB RENDER_HEADERS "*.hpp" "*.h" )

add_library(cruelRender ${LIBRARY_TYPE} ${VULKAN_CORE_SRC} ${RENDER_TASKS} ${RENDER_SRC})

target_precompile_headers(cruelRender PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelRender cruelDebug cruelWindow cruelScene ${Vulkan_LIBRARIES} ${glm_LIBRARIES})