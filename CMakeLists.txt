cmake_minimum_required(VERSION 3.1)
project(bg_calc VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(bg_calc src/hs_board.cpp src/main.cpp src/minions.cpp)

target_include_directories(bg_calc PRIVATE include)
