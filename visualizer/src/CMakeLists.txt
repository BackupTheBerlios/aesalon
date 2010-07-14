add_definitions(-W -Wall)

if(devBuild)
    add_definitions(-g)
endif(devBuild)

add_definitions(-DAesalonVersion="${aesalonVersion}")

include_directories(.)

aux_source_directory(. dotSources)
aux_source_directory(main mainSources)
aux_source_directory(session sessionSources)
aux_source_directory(module moduleSources)
aux_source_directory(interface interfaceSources)

set(sources
    ${dotSources}
    ${mainSources}
    ${sessionSources}
    ${moduleSources}
    ${interfaceSources}
)

find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)
include(${QT_USE_FILE})
QT4_AUTOMOC(${sources})

add_executable(aesalon_visualizer ${sources})
target_link_libraries(aesalon_visualizer ${QT_LIBRARIES})
