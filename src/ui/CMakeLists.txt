file(GLOB SRC "*.cpp" "*.hpp" "*.h" "../../external/imgui/*.cpp")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelGui SHARED ${SRC})

target_link_libraries(cruelGui cruelRender)