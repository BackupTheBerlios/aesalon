include(version_info)

add_definitions(-std=c++0x -W -Wall -ansi -pedantic -Wno-unused-parameter -Wno-long-long -g -pthread)
add_definitions(-DAESALON_MAJOR_VERSION=${AESALON_MAJOR_VERSION} -DAESALON_MINOR_VERSION=${AESALON_MINOR_VERSION} -DAESALON_PATCHLEVEL=${AESALON_PATCHLEVEL} -DAESALON_PLATFORM=${AESALON_PLATFORM})
add_definitions(-DAESALON_PLATFORM_x86=0 -DAESALON_PLATFORM_x86_64=1)

add_subdirectory(misc)
add_subdirectory(platform)

if(BUILD_PLATFORM)
    add_subdirectory(interface)
endif(BUILD_PLATFORM)

if(BUILD_GUI)
    add_subdirectory(gui)
endif(BUILD_GUI)

