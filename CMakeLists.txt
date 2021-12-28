cmake_minimum_required(VERSION 3.16)
project(pixel-logic)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp) # A better way to do this when we have more files is to have main.cpp be the only thing really here, the rest
# being variables that lead to different subdirectories. So we could have a variable set for all of the files related to the renderer
# or something, I don't know
add_executable(pixel-logic src/${SOURCE_FILES})
target_link_libraries(pixel-logic ${SDL2_LIBRARIES})