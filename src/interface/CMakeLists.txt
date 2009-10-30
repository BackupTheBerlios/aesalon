aux_source_directory(. SOURCES)

include_directories(..)
link_directories(..)

add_subdirectory(gdb)

add_executable(aesalon ${SOURCES} ${GDB_SOURCES})

target_link_libraries(aesalon aesalon_platform aesalon_misc pthread aesalon_gdb_interface)

