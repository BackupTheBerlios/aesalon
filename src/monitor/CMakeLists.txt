aux_source_directory(. SOURCES)
aux_source_directory(dwarf DWARF_SOURCES)
aux_source_directory(elf ELF_SOURCES)
aux_source_directory(ptrace PTRACE_SOURCES)
aux_source_directory(asm ASM_SOURCES)

include_directories(. ..)
link_directories(..)

add_executable(aesalon ${SOURCES} ${ELF_SOURCES} ${DWARF_SOURCES} ${PTRACE_SOURCES} ${ASM_SOURCES})

target_link_libraries(aesalon aesalon_platform aesalon_misc)

# This won't be needed yet . . .
#set_target_properties(aesalon PROPERTIES VERSION ${AESALON_MAJOR_VERSION}.${AESALON_MINOR_VERSION}.${AESALON_PATCHLEVEL})

install(TARGETS aesalon RUNTIME DESTINATION bin)
