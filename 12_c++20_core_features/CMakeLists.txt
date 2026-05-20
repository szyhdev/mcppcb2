cmake_minimum_required(VERSION 3.22)

project(12_c++20_core_features VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB headers *.h)
add_executable(chapter12 main.cpp ${headers})
include_directories(${CMAKE_SOURCE_DIR}/include)