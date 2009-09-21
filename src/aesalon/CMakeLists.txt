add_definitions(-std=c++0x -W -Wall -ansi -pedantic -Wno-unused-parameter -g)

add_subdirectory(misc)
add_subdirectory(comm)
add_subdirectory(display)

aux_source_directory(. SOURCES)

add_executable(aesalon ${SOURCES})

target_link_libraries(aesalon aesalon_misc aesalon_comm aesalon_display dl)


