cmake_minimum_required(VERSION 3.6)
project(lab5)

set(CMAKE_CXX_STANDARD 11)

#### MPI
find_package(MPI REQUIRED)
if (MPI_FOUND)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
else (MPI_FOUND)
    message(SEND_ERROR "This application cannot compile without MPI")
endif (MPI_FOUND)

set(SOURCE_FILES main.cpp)

add_executable(lab5 ${SOURCE_FILES})

target_link_libraries(lab5 ${MPI_LIBRARIES})
