cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

project(usbCam)

add_executable(calibrate main.cpp calibrate.cpp)

target_link_libraries(calibrate ${OpenCV_LIBS})