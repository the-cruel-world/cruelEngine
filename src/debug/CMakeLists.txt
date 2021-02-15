file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelDebug SHARED ${SRC})

target_link_libraries(cruelDebug)