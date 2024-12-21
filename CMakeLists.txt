cmake_minimum_required(VERSION 3.13)

project(codecrafters-interpreter)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp src/Tokenization/*.cpp src/Tokenization/*.hpp src/Scanner/*.cpp src/Scanner/*.cpp src/Parser/*.hpp src/Parser/*.hpp)

add_executable(interpreter ${SOURCE_FILES})