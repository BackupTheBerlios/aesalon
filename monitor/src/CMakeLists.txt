# Enable all compiler warnings, for clean code.
add_definitions(-W -Wall)

# Add debugging information if this is a development build.
if(devBuild)
    add_definitions(-g)
endif(devBuild)

# Add `pwd` as an include directory.
include_directories(.)

# Add platform flags and defines.
message(STATUS "Building for platform: ${platform}")

if(platform STREQUAL "x86")
    add_definitions(-DAesalonPlatform_x86)
elseif(platform STREQUAL "x86_64")
    add_definitions(-DAesalonPlatform_x86_64)
else(platform STREQUAL "x86")
    message(FATAL_ERROR "Cannot build for platform ${platform}; unknown or not yet implemented.")
endif(platform STREQUAL "x86")

# Define the version of Aesalon being compiled.
add_definitions(-DAesalonVersion="${aesalonVersion}")

# Define the paths to the various configuration files.
add_definitions(-DAesalonGlobalConfig="${globalConfig}" -DAesalonUserConfig="${userConfig}" -DAesalonLocalConfig="${localConfig}")

# Use CMake's built-in source-file detection to find source files.
aux_source_directory(. dotSources)
aux_source_directory(misc miscSources)
aux_source_directory(module moduleSources)
aux_source_directory(moduleInterface moduleInterfaceSources)
aux_source_directory(program programSources)

# Add the executable "aesalon".
add_executable(aesalon ${dotSources} ${miscSources} ${moduleSources} ${programSources} ${moduleInterfaceSources})

# Link aesalon with the dl and rt libraries.
target_link_libraries(aesalon dl rt)

