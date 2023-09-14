cmake_minimum_required(VERSION 3.5.0)
project(t_core)

link_directories(${GTKMM_LIBRARY_DIRS})

include_directories(${GTKMM_INCLUDE_DIRS})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/inc)

file(GLOB_RECURSE SRC_FILES *.cpp *.hpp)
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES})
