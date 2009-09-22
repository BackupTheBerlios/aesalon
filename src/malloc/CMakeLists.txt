aux_source_directory(. SOURCES)
aux_source_directory(overload OVERLOAD_SOURCES)

add_library(aesalon_bootstrap SHARED ${SOURCES})
add_library(aesalon_overload SHARED ${OVERLOAD_SOURCES})

target_link_libraries(aesalon_bootstrap dl)

