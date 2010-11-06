cmake_minimum_required(VERSION 2.8)

project(aesalon CXX)

include_directories(include)

add_definitions(-W -Wall)

include(build.config)

add_subdirectory(monitor .mbuild)

