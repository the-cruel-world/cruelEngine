file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(realPhysics SHARED ${SRC})

target_link_libraries(realPhysics m)