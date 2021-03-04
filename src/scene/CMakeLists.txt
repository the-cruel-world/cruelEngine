file(GLOB SRC "*.cpp" "*.hpp" "*.h" "basic/*.cpp" "basic/*.hpp" "objects/*.cpp" "objects/*.hpp")
file(GLOB HEADERS "*.hpp" "*.h" "basic/*.hpp" "objects/*.hpp")

add_library(cruelScene SHARED ${SRC})

target_link_libraries(cruelScene cruelVulkan ${glm_LIBRARIES})