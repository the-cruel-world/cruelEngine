file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelWindow ${LIBRARY_TYPE} ${SRC})

target_link_libraries(cruelWindow glfw)