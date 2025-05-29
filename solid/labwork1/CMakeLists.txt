cmake_minimum_required(VERSION 3.10)
project(file_stats)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(file_stats lib/file_stats.cpp)

add_executable(file_stats_app bin/main.cpp)

target_link_libraries(file_stats_app file_stats)