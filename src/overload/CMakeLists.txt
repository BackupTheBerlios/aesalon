add_definitions(-W -Wall -ansi -pedantic -D_GNU_SOURCE -masm=intel -fpic -fPIC)

aux_source_directory(. SOURCES)

add_library(aesalon_overload SHARED ${SOURCES})

target_link_libraries(aesalon_overload dl)

