# Use some CMake 2.6-specific features . . .
cmake_minimum_required(VERSION 2.6)

# Project name is "aesalon", and the languages used are C and C++.
project(aesalon C CXX)

# Include the build configuration.
include(build.config)

# Add the common include directory
include_directories(common/include)

# Add the source subdirectories.
add_subdirectory(monitor monitor/build)
#add_subdirectory(module module/build)
add_subdirectory(visualizer visualizer/build)
add_subdirectory(tests tests/build)
