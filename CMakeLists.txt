cmake_minimum_required(VERSION 3.16)
project(Chip8_Interpreter)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

add_executable(Chip8_Interpreter main.cpp Chip8.cpp Chip8.h Platform.cpp Platform.hpp)

target_compile_options(Chip8_Interpreter PRIVATE -Wall)

target_link_libraries(Chip8_Interpreter PRIVATE SDL2::SDL2)