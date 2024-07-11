cmake_minimum_required(VERSION 3.28)
project(audio_project_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(audio_project_cpp main.cpp
        include/audio.h
        cpp/utils.cpp)
find_package(portaudio CONFIG REQUIRED)
target_link_libraries(audio_project_cpp PRIVATE portaudio_static)