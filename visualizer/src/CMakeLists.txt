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
aux_source_directory(renderer rendererSources)
aux_source_directory(storage storageSources)
aux_source_directory(visualization visualizationSources)

set(sources
    ${dotSources}
    ${mainSources}
    ${sessionSources}
    ${moduleSources}
    ${rendererSources}
    ${storageSources}
    ${visualizationSources}
)

find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)
include(${QT_USE_FILE})
QT4_AUTOMOC(${sources})

qt4_add_resources(qrcSources visualizerResources.qrc)

add_executable(aesalon_visualizer ${sources} ${qrcSources})
target_link_libraries(aesalon_visualizer ${QT_LIBRARIES})
