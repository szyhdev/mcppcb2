cmake_minimum_required(VERSION 3.22)

project(09_robustness_and_performance VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB headers *.h)
add_executable(chapter09 main.cpp ${headers})
include_directories(${CMAKE_SOURCE_DIR}/include)