add_definitions(-W -Wall -ansi -pedantic -g -D_GNU_SOURCE -masm=intel -fPIC)

aux_source_directory(. SOURCES)

add_library(aesalon_overload SHARED ${SOURCES})

target_link_libraries(aesalon_overload dl)

