file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelApplication ${LIBRARY_TYPE} ${SRC})

target_precompile_headers(cruelApplication PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelApplication cruelWindow)