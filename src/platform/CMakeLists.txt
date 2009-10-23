aux_source_directory(. SOURCES)

include_directories(..)
link_directories(..)

add_library(aesalon_platform ${SOURCES})

target_link_libraries(aesalon_platform aesalon_misc)
