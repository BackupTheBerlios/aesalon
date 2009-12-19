aux_source_directory(. SOURCES)
aux_source_directory(dwarf DWARF_SOURCES)
aux_source_directory(elf ELF_SOURCES)

include_directories(..)
link_directories(..)

add_executable(aesalon ${SOURCES} ${ELF_SOURCES} ${DWARF_SOURCES})

target_link_libraries(aesalon aesalon_platform aesalon_misc pthread)

