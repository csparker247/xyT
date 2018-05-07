cmake_minimum_required(VERSION 3.5)
project(xyT)

find_package(OpenCV 3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)

set(sources
    main.cpp
)
add_executable(xyT ${sources})

target_link_libraries(xyT
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    opencv_videoio
    Boost::filesystem
)
