cmake_minimum_required(VERSION 2.8)
project(main)

set(CMAKE_CXX_STANDARD 14)

find_package(fmt)
include_directories(./include)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(main main.cpp mv_video_capture.cpp function.cpp serialport.cpp)
target_link_libraries(main 
    fmt::fmt
    /home/oyc/linuxSDK/lib/x64/libMVSDK.so
    ${OpenCV_LIBS} 
) 
