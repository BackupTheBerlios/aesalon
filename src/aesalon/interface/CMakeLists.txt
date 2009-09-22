aux_source_directory(. SOURCES)

include_directories(..)
link_directories(..)

add_library(aesalon_interface ${SOURCES})

target_link_libraries(aesalon_interface aesalon_misc)
