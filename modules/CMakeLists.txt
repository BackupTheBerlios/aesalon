include(build_list)

add_definitions(-W -Wall)

aux_source_directory(moduleInterface moduleInterfaceSources)
add_library(moduleInterface ${moduleInterfaceSources})

foreach(module ${aesalonModules})
    aux_source_directory(${module}/module moduleSources)
    add_library(${module}Module SHARED ${moduleSources})
    target_link_libraries(${module}Module moduleInterface)

#    aux_source_directory(${module}/monitor monitorSources)
#    add_library(${module}Monitor SHARED ${monitorSources})
endforeach(module)

