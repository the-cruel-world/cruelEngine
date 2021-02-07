file(GLOB SRC "*.cpp" "*.hpp" "*.h")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(realApplication SHARED ${SRC})

target_link_libraries(realApplication realWindow)