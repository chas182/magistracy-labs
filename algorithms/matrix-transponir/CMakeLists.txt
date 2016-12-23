cmake_minimum_required(VERSION 3.6)
project(matrix_transponir)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES HelpMe.h)
set(SOURCE_FILES helpme.cpp)
set(SOURCE_FILES main.cpp)
add_executable(matrix_transponir ${SOURCE_FILES})