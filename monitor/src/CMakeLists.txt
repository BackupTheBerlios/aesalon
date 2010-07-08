add_definitions(-W -Wall)

include_directories(.)

if(aesalonPlatform EQUAL "x86")
    add_definitions(-DAesalonPlatform_x86)
elseif(aesalonPlatform EQUAL "x86_64")
    add_definitions(-DAesalonPlatform_x86_64)
endif(aesalonPlatform EQUAL "x86")

add_definitions(-DAesalonVersion="${aesalonVersion}")

aux_source_directory(. dotSources)
aux_source_directory(misc miscSources)
aux_source_directory(module moduleSources)
aux_source_directory(environment environmentSources)

add_executable(aesalon ${dotSources} ${miscSources} ${moduleSources} ${environmentSources})

target_link_libraries(aesalon dl rt)

