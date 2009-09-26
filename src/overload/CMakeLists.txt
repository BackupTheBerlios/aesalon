execute_process(COMMAND ./detect_platform.sh OUTPUT_VARIABLE PLATRFORM OUTPUT_STRIP_TRAILING_WHITESPACE)

add_definitions(-W -Wall -ansi -pedantic -g -D_GNU_SOURCE -DAESALON_PLATFORM=PLATFORM -masm=intel -fPIC)

aux_source_directory(. SOURCES)

add_library(aesalon_overload SHARED ${SOURCES})

target_link_libraries(aesalon_overload dl)

