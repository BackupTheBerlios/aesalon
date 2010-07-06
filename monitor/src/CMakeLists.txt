add_definitions(-W -Wall)

if(aesalonPlatform EQUAL "x86")
    add_definitions(-DAesalonPlatform_x86)
elseif(aesalonPlatform EQUAL "x86_64")
    add_definitions(-DAesalonPlatform_x86_64)
endif(aesalonPlatform EQUAL "x86")

add_definitions(-DAesalonVersion="${aesalonVersion}")

aux_source_directory(. dotSources)
aux_source_directory(dll dllSources)

add_executable(aesalon ${dotSources} ${dllSources})

target_link_libraries(aesalon dl rt)

