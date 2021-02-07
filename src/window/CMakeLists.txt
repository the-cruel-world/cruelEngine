file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(realWindow SHARED ${SRC})

target_link_libraries(realWindow glfw)