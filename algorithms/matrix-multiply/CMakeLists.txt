cmake_minimum_required(VERSION 3.6)
project(matrix_multiply)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES HelpMe.h main.cpp helpme.cpp)
add_executable(matrix_multiply ${SOURCE_FILES})