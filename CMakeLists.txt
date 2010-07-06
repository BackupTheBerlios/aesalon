# Use some CMake 2.6-specific features . . .
cmake_minimum_required(VERSION 2.6)

# Aesalon is written in C and C++; enable support for both.
enable_language(C)
enable_language(CXX)

# Project name is "aesalon".
project(aesalon)

# Include the build configuration.
include(build.config)

add_subdirectory(monitor build)
