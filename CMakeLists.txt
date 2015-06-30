cmake_minimum_required(VERSION 3.2)
project(xyT)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${OpenCV_INCLUDE_DIR})

set(SOURCE_FILES main.cpp)
add_executable(xyT ${SOURCE_FILES})

target_link_libraries(xyT ${OpenCV_LIBS})