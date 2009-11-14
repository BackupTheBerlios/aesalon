aux_source_directory(. SOURCES)

remove_definitions(-std=c++0x)
add_definitions(-std=c99)

add_library(aesalon_overload SHARED ${SOURCES})

