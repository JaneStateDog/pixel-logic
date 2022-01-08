cmake_minimum_required(VERSION 3.16)
project(pixel-logic)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(pixel-logic src/main.c src/pixel/pixel.c src/pixel/pixel.h)
target_link_libraries(pixel-logic ${SDL2_LIBRARIES})