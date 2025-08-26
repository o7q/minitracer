cmake_minimum_required(VERSION 3.5)
project(raytracer)

file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -Wall)

find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} raylib m)
