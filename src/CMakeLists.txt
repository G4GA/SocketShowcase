project(core_src)

set(PROJECT_SOURCES socket_handler.c
                    gui.c
                    display_info.c)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})


target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../raylib/build/include
    )

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../raylib/build/raylib)

target_link_libraries(${PROJECT_NAME} PRIVATE
    rt
    raylib
    )
