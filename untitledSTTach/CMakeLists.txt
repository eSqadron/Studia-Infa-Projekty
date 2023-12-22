cmake_minimum_required(VERSION 3.15)
project(untitledSTTach)

set(CMAKE_CXX_STANDARD 20)

add_library(untitledSTTach  library.h )

add_executable(library.cpp main.cpp)