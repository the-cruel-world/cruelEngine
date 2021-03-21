file(GLOB SRC "*.cpp" "*.hpp" "*.h" "basic/*.cpp" "basic/*.hpp" "scene_objects/*.cpp" "scene_objects/*.hpp")
file(GLOB HEADERS "*.hpp" "*.h" "basic/*.hpp" "scene_objects/*.hpp")

add_library(cruelScene SHARED ${SRC})

target_link_libraries(cruelScene ${glm_LIBRARIES})