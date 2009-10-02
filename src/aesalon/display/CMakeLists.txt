aux_source_directory(. SOURCES)

include_directories(.. ../..)

if(DEFINED BUILD_STDOUT_INTERFACE)
    aux_source_directory(stdout STDOUT_SOURCES)
    add_definitions(-DAESALON_BUILD_STDOUT_INTERFACE)
endif(DEFINED BUILD_STDOUT_INTERFACE)

add_library(aesalon_display ${SOURCES} ${STDOUT_SOURCES})

