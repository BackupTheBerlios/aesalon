add_definitions(-W -Wall)

if(devBuild)
    add_definitions(-g)
endif(devBuild)

include_directories(.)

message(STATUS "Building for platform: ${platform}")

if(platform STREQUAL "x86")
    add_definitions(-DAesalonPlatform_x86)
elseif(platform STREQUAL "x86_64")
    add_definitions(-DAesalonPlatform_x86_64)
else(platform STREQUAL "x86")
    message(FATAL_ERROR "No known platform specified in build configuration. Given ${platform}.")
endif(platform STREQUAL "x86")

add_definitions(-DAesalonVersion="${aesalonVersion}")

add_definitions(-DAesalonGlobalConfig="${globalConfig}" -DAesalonUserConfig="${userConfig}" -DAesalonLocalConfig="${localConfig}")

aux_source_directory(. dotSources)
aux_source_directory(misc miscSources)
aux_source_directory(module moduleSources)
aux_source_directory(program programSources)

add_executable(aesalon ${dotSources} ${miscSources} ${moduleSources} ${programSources})

target_link_libraries(aesalon dl rt)

