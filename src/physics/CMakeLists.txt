file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelPhysics SHARED ${SRC})

target_link_libraries(cruelPhysics m)