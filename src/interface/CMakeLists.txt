aux_source_directory(. SOURCES)
aux_source_directory(dwarf DWARF_SOURCES)

include_directories(..)
link_directories(..)

add_executable(aesalon ${SOURCES} ${DWARF_SOURCES})

target_link_libraries(aesalon aesalon_platform aesalon_misc pthread)

