cmake_minimum_required(VERSION 3.15)
project(pmt)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -O2)
set(EXECUTABLE_OUTPUT_PATH ./bin)

add_executable(pmt src/main.cpp src/aho_corasick.cpp src/aho_corasick.h src/shift_or.h src/shift_or.cpp src/shift_and.cpp src/shift_and.h src/wu_manber.cpp src/wu_manber.h src/sellers.cpp src/sellers.h)