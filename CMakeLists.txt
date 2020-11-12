cmake_minimum_required(VERSION 3.5)

project(clcap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(clcap main.cpp)
target_link_libraries(clcap ${OpenCV_LIBS})

install(TARGETS clcap RUNTIME DESTINATION bin)
