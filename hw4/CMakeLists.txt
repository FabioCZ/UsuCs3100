cmake_minimum_required(VERSION 2.8.4)
project(hw4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp shell.h)
add_executable(hw4 ${SOURCE_FILES})