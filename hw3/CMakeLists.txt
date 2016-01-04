cmake_minimum_required(VERSION 2.8.4)
project(hw3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")

set(SOURCE_FILES main.cpp Calculators.h Mandelbrot.h)
add_executable(hw3 ${SOURCE_FILES})