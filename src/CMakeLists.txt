include(version_info)

add_definitions(-std=c++0x -W -Wall -ansi -pedantic -Wno-unused-parameter -g -pthread -DAESALON_MAJOR_VERSION=${MAJOR_VERSION} -DAESALON_MINOR_VERSION=${MINOR_VERSION} -DAESALON_PATCHLEVEL=${PATCHLEVEL})

add_subdirectory(misc)
add_subdirectory(platform)
add_subdirectory(interface)

include_directories(. ..)

aux_source_directory(. SOURCES)

