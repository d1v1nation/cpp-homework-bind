cmake_minimum_required(VERSION 3.6)
project(bind)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES main.cpp bind.h)
add_executable(bind ${SOURCE_FILES})
