cmake_minimum_required(VERSION 3.6)
project(lab2)
find_package (Threads)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(lab2 ${SOURCE_FILES})
target_link_libraries (lab2 ${CMAKE_THREAD_LIBS_INIT})