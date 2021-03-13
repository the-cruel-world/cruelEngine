FIND_PACKAGE(Vulkan REQUIRED FATAL_ERROR)

file (GLOB VULKAN_CORE_SRC "vulkan/*.cpp" "vulkan/*.hpp" "vulkan/*.h")

file (GLOB VULKAN_CORE_HEADERS "vulkan/*.hpp" "vulkan/*.h")

file (GLOB RENDER_SRC "*.cpp" "*.hpp" "*.h" )

file (GLOB RENDER_HEADERS "*.hpp" "*.h" )

include_directories(../ ./
    ${Vulkan_INCLUDE_DIRS}
    ${glfw3_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS})

add_library(cruelRender SHARED ${VULKAN_CORE_SRC} ${RENDER_SRC})

target_link_libraries(cruelRender cruelDebug cruelWindow cruelScene ${Vulkan_LIBRARIES} ${glm_LIBRARIES})