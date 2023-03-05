cmake_minimum_required(VERSION 2.8)
project(main)

find_package(OpenCV REQUIRED)

include_directories(include)
add_subdirectory(src)
add_executable(main src/main.cpp)

target_link_libraries(main ${OpenCV_LIBS} assets)