cmake_minimum_required(VERSION 3.14)
project(ThreadedLinuxApp)

set(CMAKE_CXX_STANDARD 14 -pthread)

add_executable(ThreadedLinuxApp main.cpp)