cmake_minimum_required(VERSION 3.10)

project(chell)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)

add_executable(chell main.cpp ./sources/shell.cpp)
