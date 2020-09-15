cmake_minimum_required(VERSION 3.16)
project(emulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(emulator main.cpp Register.cpp Register.h FlagRegister.cpp FlagRegister.h constants.h)