cmake_minimum_required(VERSION 3.5.0)
add_subdirectory(sorter_core)

if(Linux)
  message("Linux build")
  add_subdirectory(sorter_app/linux EXCLUDE_FROM_ALL)
endif()

if(Windows)
  message("Windows build")
endif()
