aux_source_directory(. SOURCES)
aux_source_directory(dwarf DWARF_SOURCES)
aux_source_directory(elf ELF_SOURCES)
aux_source_directory(ptrace PTRACE_SOURCES)

include_directories(. ..)
link_directories(..)

add_executable(aesalon ${SOURCES} ${ELF_SOURCES} ${DWARF_SOURCES} ${PTRACE_SOURCES})

target_link_libraries(aesalon aesalon_platform aesalon_misc)

