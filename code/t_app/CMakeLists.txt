cmake_minimum_required(VERSION 3.5.0)
project(t_app)

link_directories(${GTKMM_LIBRARY_DIRS})

include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${PROJECT_ROOT}/code/t_core/inc)

target_link_libraries(${PROJECT_NAME} t_core ${GTKMM_LIBRARIES})
