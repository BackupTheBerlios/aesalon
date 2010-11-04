aux_source_directory(. dotSources)

add_executable(aesalon ${dotSources})

target_link_libraries(aesalon dl rt)
