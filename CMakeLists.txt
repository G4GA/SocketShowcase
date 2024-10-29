cmake_minimum_required(VERSION 3.28)

project(SocketShowcase)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON) 
set(CMAKE_C_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/raylib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core_src
    )
