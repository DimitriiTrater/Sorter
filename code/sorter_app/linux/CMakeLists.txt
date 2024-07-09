cmake_minimum_required(VERSION 3.5.0)
project(linux)

message("Create linux build")

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${PROJECT_ROOT}/code/sorter_core/inc)

target_link_libraries(${PROJECT_NAME} sorter_core Qt5::Widgets)
