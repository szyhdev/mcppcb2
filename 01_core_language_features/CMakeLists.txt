cmake_minimum_required(VERSION 3.22)

project(01_core_language_features VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB headers *.h)
file(GLOB sources *.cpp)
add_executable(chapter01 ${sources} ${headers})
include_directories(${CMAKE_SOURCE_DIR}/include)