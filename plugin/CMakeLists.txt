set (SOURCES plugin.cpp
        )

add_library(plugin ${LIBRARY_TYPE} ${SOURCES})

target_link_libraries(plugin)