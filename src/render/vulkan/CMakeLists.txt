FIND_PACKAGE(Vulkan REQUIRED FATAL_ERROR)

file(GLOB SRC "*.cpp" "*.hpp" "*.h")

file(GLOB HEADERS "*.hpp" "*.h")

include_directories(../ ./
    ${Vulkan_INCLUDE_DIRS}
    ${glfw3_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS})

add_library(cruelVulkan SHARED ${SRC}  "../render_context.cpp")

target_link_libraries(cruelVulkan cruelWindow cruelDebug ${Vulkan_LIBRARIES} ${glm_LIBRARIES})