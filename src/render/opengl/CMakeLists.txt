file(GLOB SRC "*.cpp" "*.hpp" "*.h"  "../renderContext.cpp")
file(GLOB HEADERS "*.hpp" "*.h")

add_library(cruelOpenGL SHARED ${SRC})

target_link_libraries(cruelOpenGL cruelWindow cruelScene glfw ${glm_LIBRARIES})