aux_source_directory(. SOURCES)

add_library(aesalon_malloc SHARED ${SOURCES})

target_link_libraries(aesalon_malloc dl)

