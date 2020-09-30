cmake_minimum_required(VERSION 3.16)
project(emulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(emulator main.cpp Components/Register.cpp Components/Register.h Components/FlagRegister.cpp Components/FlagRegister.h Components/constants.h Components/Memory.cpp Components/Memory.h Components/Ports.cpp Components/Ports.h Components/DataBus.cpp Components/DataBus.h Components/AddressBus.cpp Components/AddressBus.h Assembler/Parser.cpp Assembler/Parser.h Assembler/Tokenizer.cpp Assembler/Tokenizer.h Assembler/Assembler.cpp Assembler/Assembler.h InstructionSet/Instructions.h)
add_executable(assembler assembler_main.cpp  Components/constants.h Assembler/Parser.cpp Assembler/Parser.h Assembler/Tokenizer.cpp Assembler/Tokenizer.h Assembler/Assembler.cpp Assembler/Assembler.h InstructionSet/Instructions.h)