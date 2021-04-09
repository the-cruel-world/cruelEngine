file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelDebug ${LIBRARY_TYPE} ${SRC})

target_precompile_headers(cruelDebug PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelDebug)