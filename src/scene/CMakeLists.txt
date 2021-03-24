file(GLOB SRC "*.cpp" "*.hpp" "*.h" "basic/*.cpp" "basic/*.hpp" "components/*.cpp" "components/*.hpp")
file(GLOB HEADERS "*.hpp" "*.h" "basic/*.hpp" "components/*.hpp")

add_library(cruelScene SHARED ${SRC})

target_link_libraries(cruelScene ${glm_LIBRARIES})