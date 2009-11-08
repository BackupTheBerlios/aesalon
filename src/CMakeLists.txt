include(version_info)

add_definitions(-std=c++0x -W -Wall -ansi -pedantic -Wno-unused-parameter -Wno-long-long -g -pthread)
add_definitions(-DAESALON_MAJOR_VERSION=${AESALON_MAJOR_VERSION} -DAESALON_MINOR_VERSION=${AESALON_MINOR_VERSION} -DAESALON_PATCHLEVEL=${AESALON_PATCHLEVEL} -DAESALON_PLATFORM=${AESALON_PLATFORM})
add_definitions(-DAESALON_PLATFORM_x86=0 -DAESALON_PLATFORM_x86_64=1)

add_subdirectory(misc)
add_subdirectory(platform)
add_subdirectory(interface)

# Now, piggyback onto qmake for the gui system; as qmake removes all the hassles of automoc.
#add_custom_target(gui /usr/bin/make -f qmake.makefile WORKING_DIRECTORY src/gui)

add_subdirectory(gui)

