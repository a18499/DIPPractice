cmake_minimum_required(VERSION 2.8)
project( Interploation )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Interploation interpolation.cpp )
target_link_libraries( Interploation ${OpenCV_LIBS} )
