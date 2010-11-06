aux_source_directory(. dotSources)
aux_source_directory(config configSources)

set(sources)

include(sourceDirectories)

foreach(dir ${directories})
	aux_source_directory(${dir} tempSources)
	set(sources ${sources} ${tempSources})

endforeach(dir)

add_executable(aesalon ${dotSources} ${configSources})

target_link_libraries(aesalon dl rt)
