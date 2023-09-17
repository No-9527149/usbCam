cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

project(usbCam)

add_subdirectory(utils)
add_subdirectory(calibrate)

link_directories("/usr/lib/x86_64-linux-gnu")

add_executable(usbCam main.cpp)
target_link_libraries(usbCam utils)
target_link_libraries(usbCam libjpeg.so libSDL2.so)