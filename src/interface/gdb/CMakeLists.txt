execute_process(COMMAND /bin/rm -f src/interface/gdb/parse.cc)

add_custom_command(OUTPUT parse.cc COMMAND /usr/bin/bisonc++ GDB.y MAIN_DEPENDENCY GDB.y)

aux_source_directory(. GDB_SOURCES)

add_library(aesalon_gdb_interface ${GDB_SOURCES})

remove_definitions(-std=c++0x -pedantic)
add_definitions(-std=gnu++98)

add_library(aesalon_gdb_bison_parser parse.cc)

target_link_libraries(aesalon_gdb_interface aesalon_gdb_bison_parser)

remove_definitions(-std=gnu++98)
#add_definitions(-std=c++0x -pedantic)

