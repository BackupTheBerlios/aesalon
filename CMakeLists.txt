# There are some CMake 2.6-specific features used . . .
cmake_minimum_required(VERSION 2.6)

# If the cmake_policy command exists, allow relative library paths
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# aesalon uses C++, not C; enable C++ support
enable_language(CXX)

# The project name is aesalon
project(aesalon)

# Include the build configuration file
include(build.config)

# Generate version information if this is a git repository
if(DEVELOPMENT_BUILD)
    execute_process(COMMAND ./version_info.sh)
endif(DEVELOPMENT_BUILD)

# Include auto-generated version information
include(version_info)

# Set up a few options, depending on the build type
if(DEVELOPMENT_BUILD)
#    set(CMAKE_INSTALL_PREFIX "install/")
    set(CMAKE_BUILD_TYPE Debug)
else(DEVELOPMENT_BUILD)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif(DEVELOPMENT_BUILD)


# Now compile everything . . .
add_subdirectory(src)

