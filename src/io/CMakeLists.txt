file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelIO SHARED ${SRC})

target_link_libraries(cruelIO)