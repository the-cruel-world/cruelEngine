file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelEvents SHARED ${SRC})

target_link_libraries(cruelEvents)