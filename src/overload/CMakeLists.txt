add_definitions(-D_GNU_SOURCE)

aux_source_directory(. SOURCES)

add_library(aesalon_overload SHARED ${SOURCES})

target_link_libraries(aesalon_overload dl)

