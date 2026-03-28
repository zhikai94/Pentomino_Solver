cmake_minimum_required(VERSION 3.15)
include_guard(GLOBAL)
project(pentomino)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenCV_DIR "/home/zhikai/Code/3rd_party/opencv_lib/lib/cmake/opencv4/")
set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

add_executable(pentomino
    pento.cpp
)

target_link_libraries(pentomino
    ${OpenCV_LIBS}
)