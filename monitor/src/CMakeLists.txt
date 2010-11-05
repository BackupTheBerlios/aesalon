aux_source_directory(. dotSources)
aux_source_directory(configuration configurationSources)

add_executable(aesalon ${dotSources} ${configurationSources})

target_link_libraries(aesalon dl rt)
