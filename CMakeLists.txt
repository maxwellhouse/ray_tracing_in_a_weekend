cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project("Ray tracing in a weekend")

add_executable(raytracer main.cpp)
target_link_libraries(raytracer m dl)