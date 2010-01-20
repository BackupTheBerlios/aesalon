add_definitions(-std=c++0x -W -Wall -ansi -pedantic -Wno-unused-parameter -Wno-long-long -pthread -DDEFAULT_PORT=${DEFAULT_PORT})
add_definitions(-DAESALON_MAJOR_VERSION=${AESALON_MAJOR_VERSION} -DAESALON_MINOR_VERSION=${AESALON_MINOR_VERSION} -DAESALON_PATCHLEVEL=${AESALON_PATCHLEVEL} -DAESALON_PLATFORM=${AESALON_PLATFORM})
add_definitions(-DAESALON_PLATFORM_x86=0 -DAESALON_PLATFORM_x86_64=1)

if(DEVELOPMENT_BUILD)
    add_definitions(-DAESALON_DEV_BUILD)
endif(DEVELOPMENT_BUILD)

add_subdirectory(misc)
add_subdirectory(platform)

if(BUILD_MONITOR)
    add_subdirectory(monitor)
endif(BUILD_MONITOR)

if(BUILD_GUI)
    add_subdirectory(gui)
endif(BUILD_GUI)

