cmake_minimum_required(VERSION 3.5)
project(minitracer)

add_executable(${PROJECT_NAME} main.c)

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -Wall)

find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} raylib m)
