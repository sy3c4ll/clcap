cmake_minimum_required(VERSION 3.5)

project(seeker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(seeker main.cpp)
target_link_libraries(seeker ${OpenCV_LIBS})

install(TARGETS seeker RUNTIME DESTINATION bin)
