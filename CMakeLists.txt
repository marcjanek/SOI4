cmake_minimum_required(VERSION 3.12)
project(SOI4_1)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(SOI4_1 main.cpp monitor.h)