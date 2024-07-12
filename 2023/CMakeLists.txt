cmake_minimum_required(VERSION 3.22)

project(advent-of-code)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-march=native -g -Wall -Werror -Wextra -Wconversion -Wpedantic -Wshadow")

add_executable(d1p1
    d1p1.cpp
)

add_executable(d1p2
    d1p2.cpp
)
