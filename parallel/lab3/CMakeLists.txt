cmake_minimum_required(VERSION 3.6)
project(lab3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES server.cpp)
add_executable(server server.cpp)
add_executable(client client.cpp)