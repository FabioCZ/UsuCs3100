cmake_minimum_required(VERSION 2.8.4)
project(hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Calculators.h Calculators.cpp)
add_executable(hw1 ${SOURCE_FILES})