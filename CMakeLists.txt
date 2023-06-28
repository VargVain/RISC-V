cmake_minimum_required(VERSION 3.16)
project(Tomasulo)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        src/main.cpp src/utils.h src/decode.h src/instruction.h src/memory.h src/execute.h src/ROB.h src/RS.h)
