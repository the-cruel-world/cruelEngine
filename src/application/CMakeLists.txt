file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelApplication SHARED ${SRC})

target_link_libraries(cruelApplication cruelWindow)