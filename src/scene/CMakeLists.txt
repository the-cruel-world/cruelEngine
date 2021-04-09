file(GLOB SRC "*.cpp" "*.hpp" "*.h" "basic/*.cpp" "basic/*.hpp" "components/*.cpp" "components/*.hpp")
file(GLOB HEADERS "*.hpp" "*.h" "basic/*.hpp" "components/*.hpp")

set (TINYGLTF ${CMAKE_SOURCE_DIR}/external/tinygltfloader)

add_library(cruelScene ${LIBRARY_TYPE} ${SRC})

target_precompile_headers(cruelScene PUBLIC "${CMAKE_SOURCE_DIR}/src/common.h")

target_link_libraries(cruelScene ${glm_LIBRARIES})