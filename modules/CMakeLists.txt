include(build_list)

add_definitions(-W -Wall)

include_directories(.)

aux_source_directory(collectorInterface collectorInterfaceSources)
add_library(collectorInterface SHARED ${collectorInterfaceSources})
target_link_libraries(collectorInterface rt)

foreach(module ${aesalonModules})
    aux_source_directory(${module}/collector moduleSources)
    add_library(${module}Collector SHARED ${moduleSources})

#    aux_source_directory(${module}/monitor monitorSources)
#    add_library(${module}Monitor SHARED ${monitorSources})
endforeach(module)

