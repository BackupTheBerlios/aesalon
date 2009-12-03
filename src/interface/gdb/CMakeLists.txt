aux_source_directory(. GDB_SOURCES)

add_library(aesalon_gdb_interface ${GDB_SOURCES})

target_link_libraries(aesalon_gdb_interface aesalon_platform aesalon_misc)

