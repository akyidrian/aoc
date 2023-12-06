cmake_minimum_required(VERSION 3.22)

project(advent-of-code)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-march=native -g -Wall -Werror -Wextra -Wconversion -Wpedantic -Wshadow")

add_executable(day1
    day1.cpp
)

add_executable(day2
    day2.cpp
)

add_executable(day3
    day3.cpp
)

