cmake_minimum_required(VERSION 3.1)
project(bg_calc VERSION 1.0)

find_package(GTest)
find_package(Threads)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(bg_calc src/hs_board.cpp src/main.cpp src/minions.cpp)

target_include_directories(bg_calc PRIVATE include)
target_link_libraries(bg_calc Threads::Threads)

if(GTest_FOUND)
	add_executable(battle_test src/battle_test.cpp src/minions.cpp src/hs_board.cpp)
	target_include_directories(battle_test PRIVATE ${GTEST_INCLUDE_DIRS} include)
	target_link_libraries(battle_test ${GTEST_LIBRARIES} Threads::Threads)
endif()
