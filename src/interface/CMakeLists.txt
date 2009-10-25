aux_source_directory(. SOURCES)

include_directories(..)
link_directories(..)

add_executable(aesalon ${SOURCES})

target_link_libraries(aesalon aesalon_platform aesalon_misc pthread)

