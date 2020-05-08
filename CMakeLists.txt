cmake_minimum_required(VERSION 3.16)
project(Chip8_Interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chip8_Interpreter main.cpp Chip8.cpp Chip8.h)