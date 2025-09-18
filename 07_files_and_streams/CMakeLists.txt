cmake_minimum_required(VERSION 3.22)

project(07_files_and_streams VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB headers *.h)
add_executable(chapter07 main.cpp ${headers})
include_directories(${CMAKE_SOURCE_DIR}/include)