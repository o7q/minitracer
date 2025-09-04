cmake_minimum_required(VERSION 3.5)
project(minitracer)

# example setups:
# balls
# cornell_box
# infinite
# suzanne
set(EXAMPLE suzanne)

file(GLOB UTIL_SRC "examples/raylib_utils/*.c")
add_executable(${PROJECT_NAME} examples/${EXAMPLE}.c ${UTIL_SRC} minitracer.c)

target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall)

find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} raylib m)
