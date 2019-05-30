cmake_minimum_required(VERSION 3.8)
project(a)

add_definitions('-std=c++11')

file(GLOB_RECURSE sources
    "src/*.cpp"
    "src/*.cc"
    "include/*.hpp"
    "*.h"
)

find_package(OpenCV)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "not find opencv.")
endif()

add_executable(a main.cpp ${sources})
target_link_libraries(a  ${OpenCV_LIBS} -lpthread)
