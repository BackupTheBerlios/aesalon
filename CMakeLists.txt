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
    set(CMAKE_INSTALL_PREFIX "install/")
    set(CMAKE_BUILD_TYPE Debug)
else(DEVELOPMENT_BUILD)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif(DEVELOPMENT_BUILD)

# Compile the tests if wanted
if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)

# If this is a dev build, add a doc target
if(DEVELOPMENT_BUILD)
    add_custom_target("doc" DEPENDS monitor_doc gui_doc)
endif(DEVELOPMENT_BUILD)

# Compile the monitor, if wanted
if(BUILD_MONITOR)
    add_subdirectory(monitor)
endif(BUILD_MONITOR)

# Compile the GUI, if wanted
if(BUILD_GUI)
    add_subdirectory(gui)
endif(BUILD_GUI)

