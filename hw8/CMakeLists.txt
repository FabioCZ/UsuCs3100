cmake_minimum_required(VERSION 3.1)
project(hw8)

# compiled with clang on OS X
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp calculators.h sim_event.hpp sim_event.cpp sim_state.hpp sim_state.cpp simulator.hpp)
add_executable(hw8 ${SOURCE_FILES})