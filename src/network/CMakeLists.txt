file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelNetwork ${LIBRARY_TYPE} ${SRC})

target_precompile_headers(cruelNetwork PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelNetwork)