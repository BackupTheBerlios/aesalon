# Uncomment the below lines when it is time to begin building the gdb interface again

execute_process(COMMAND /bin/rm -f src/interface/gdb/parse.cc)

#add_custom_command(OUTPUT parse.cc COMMAND /usr/bin/bisonc++ GDB.y MAIN_DEPENDENCY GDB.y)

aux_source_directory(. GDB_SOURCES)

add_library(aesalon_gdb_interface ${GDB_SOURCES} )#parse.cc)

