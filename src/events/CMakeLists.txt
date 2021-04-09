file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelEvents ${LIBRARY_TYPE} ${SRC})

target_precompile_headers(cruelEvents PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelEvents)