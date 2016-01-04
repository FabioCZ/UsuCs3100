cmake_minimum_required(VERSION 2.8.4)
project(hw6)
#SET (CMAKE_CXX_COMPILER             "/usr/local/Cellar/gcc/4.9.2_1/bin/g++-4.9")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES sim_state.hpp sim_state.cpp sim_event.hpp sim_event.cpp simulator.hpp calculators.h main.cpp)
add_executable(hw6 ${SOURCE_FILES})