cmake_minimum_required(VERSION 3.5.0)

project(Sorter VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

find_package(Qt5 COMPONENTS Widgets REQUIRED)

option(Linux "Linux" OFF)

add_subdirectory(code)