cmake_minimum_required(VERSION 3.28)
project(audio_project_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(audio_project_cpp main.cpp
        include/audiofile.h
        cpp/audiofile.cpp
        include/audiostream.h
        cpp/audiostream.cpp
        cpp/synthesizer.cpp
        include/synthesizer.h)

find_package(portaudio CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)
target_link_libraries(audio_project_cpp PRIVATE SndFile::sndfile)
target_link_libraries(audio_project_cpp PRIVATE portaudio_static)