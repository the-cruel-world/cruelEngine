file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelScene SHARED ${SRC})

target_link_libraries(cruelScene ${glm_LIBRARIES})