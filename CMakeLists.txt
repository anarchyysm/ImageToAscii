cmake_minimum_required(VERSION 3.30)
project(ConvertImageToAscii)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(AsciiConversor)

add_executable(ConvertImageToAscii
    Main.cpp
    AsciiConversor/ImageProcessor.cpp
    AsciiConversor/VideoProcessor.cpp)

target_link_libraries ( ConvertImageToAscii ${OpenCV_LIBS} )
