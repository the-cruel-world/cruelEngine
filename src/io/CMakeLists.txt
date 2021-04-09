file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelIO ${LIBRARY_TYPE} ${SRC})

target_precompile_headers(cruelIO PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelIO)