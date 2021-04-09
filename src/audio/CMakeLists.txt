include_directories(../)

file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelAudio ${LIBRARY_TYPE} ${SRC})

target_precompile_headers(cruelAudio PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelAudio)