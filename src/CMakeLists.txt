include(version_info)

add_definitions(-std=c++0x -W -Wall -ansi -pedantic -Wno-unused-parameter -g -pthread -DMAJOR_VERSION=${MAJOR_VERSION} -DMINOR_VERSION=${MINOR_VERSION} -DPATCHLEVEL=${PATCHLEVEL})

add_subdirectory(misc)
add_subdirectory(platform)
add_subdirectory(interface)

include_directories(. ..)

aux_source_directory(. SOURCES)

