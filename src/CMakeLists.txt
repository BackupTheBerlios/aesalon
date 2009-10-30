include(version_info)

add_definitions(-std=c++0x -W -Wall -ansi -pedantic -Wno-unused-parameter -Wno-long-long -g -pthread -DAESALON_MAJOR_VERSION=${MAJOR_VERSION} -DAESALON_MINOR_VERSION=${MINOR_VERSION} -DAESALON_PATCHLEVEL=${PATCHLEVEL})

add_subdirectory(misc)
add_subdirectory(platform)
add_subdirectory(interface)

# Now, piggyback onto qmake for the gui system; as qmake removes all the hassles of automoc.
#add_custom_target(gui /usr/bin/make -f qmake.makefile WORKING_DIRECTORY src/gui)

add_subdirectory(gui)

