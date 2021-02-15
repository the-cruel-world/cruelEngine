file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelWindow SHARED ${SRC})

target_link_libraries(cruelWindow glfw)