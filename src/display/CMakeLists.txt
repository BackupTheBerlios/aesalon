aux_source_directory(. SOURCES)

include_directories(.. ../..)
link_directories(..)

add_library(aesalon_display ${SOURCES})

target_link_libraries(aesalon_display aesalon_interface)

