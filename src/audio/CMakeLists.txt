include_directories(../)

file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelAudio SHARED ${SRC})

target_link_libraries(cruelAudio)