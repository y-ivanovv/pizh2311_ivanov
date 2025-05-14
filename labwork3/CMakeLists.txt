cmake_minimum_required(VERSION 3.10)
project(sandpile_model)

set(CMAKE_CXX_STANDARD 17)

add_executable(sandpile_model
    src/main.cpp
    src/config.cpp
    src/grid.cpp
    src/bmp_writer.cpp
    src/sandpile.cpp
)

target_include_directories(sandpile_model PRIVATE include)