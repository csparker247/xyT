cmake_minimum_required(VERSION 3.2)
project(xyT)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${OpenCV_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set(SOURCE_FILES main.cpp)
add_executable(xyT ${SOURCE_FILES})

target_link_libraries(xyT ${OpenCV_LIBS} ${Boost_LIBRARIES})