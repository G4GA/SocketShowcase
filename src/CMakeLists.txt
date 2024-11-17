project(core_src)

set(MANAGER_LIB manager)
set(MANAGER_LIB_SRC manager.c)

set(GUI_LIB gui)
set(GUI_LIB_SRC socket_handler.c
                   gui.c
                   display_info.c)

set(SERVER_LIB server)
set(SERVER_LIB_SRC server.c)

set(LOG_LIB log)
set(LOG_LIB_SRC log.c)

add_library(${GUI_LIB} STATIC ${GUI_LIB_SRC})
add_library(${MANAGER_LIB} STATIC ${MANAGER_LIB_SRC})
add_library(${SERVER_LIB} STATIC ${SERVER_LIB_SRC})
add_library(${LOG_LIB} STATIC ${LOG_LIB_SRC})

include_directories(${MANAGER_LIB} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )

target_include_directories(${GUI_LIB} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../raylib/build/include
    )

target_link_directories(${GUI_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../raylib/build/raylib)

target_link_libraries(${GUI_LIB} PRIVATE
    rt
    raylib
    )

target_link_libraries(${SERVER_LIB} PRIVATE ${LOG_LIB})

target_link_libraries(${MANAGER_LIB} PRIVATE ${GUI_LIB}
                                             ${SERVER_LIB})
