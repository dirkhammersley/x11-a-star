cmake_minimum_required(VERSION 3.10)
project(AStar)

add_library(Window x_window.cpp x_window.h)
add_library(Grid grid.cpp grid.h)
add_executable(AStarApp main.cpp main.h)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(EXECUTABLE_OUTPUT_PATH ../build)

target_link_libraries(AStarApp Window Grid "-lX11")