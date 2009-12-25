cmake_minimum_required(VERSION 2.6)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

enable_language(CXX)
enable_language(C)

project(aesalon)

include(build.config)
if(DEVELOPMENT_BUILD)
    execute_process(COMMAND ./version_info.sh)
endif(DEVELOPMENT_BUILD)
include(version_info)

add_subdirectory(src)

