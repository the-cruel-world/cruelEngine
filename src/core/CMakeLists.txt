file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelCore ${LIBRARY_TYPE} ${SRC})

target_precompile_headers(cruelCore PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelCore)