cmake_minimum_required(VERSION 3.14)
project(Path_tracing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread -m64")

add_executable(Path_tracing main.cpp sphere.h material.h lambertian.h metal.h dielectric.h)