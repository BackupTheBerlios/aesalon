include(build_list)

add_definitions(-W -Wall)

aux_source_directory(collectorInterface collectorInterfaceSources)
add_library(collectorInterface ${collectorInterfaceSources})

foreach(module ${aesalonModules})
    aux_source_directory(${module}/collector moduleSources)
    add_library(${module}Collector SHARED ${moduleSources})
    target_link_libraries(${module}Collector collectorInterface)

#    aux_source_directory(${module}/monitor monitorSources)
#    add_library(${module}Monitor SHARED ${monitorSources})
endforeach(module)

