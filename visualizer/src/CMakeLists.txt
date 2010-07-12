add_definitions(-W -Wall)

add_definitions(-DAesalonVersion="${aesalonVersion}")

include_directories(.)

aux_source_directory(. dotSources)
aux_source_directory(main mainSources)

set(sources
    ${dotSources}
    ${mainSources}
)

find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)
include(${QT_USE_FILE})
QT4_AUTOMOC(${sources})

add_executable(aesalon_visualizer ${sources})
target_link_libraries(aesalon_visualizer ${QT_LIBRARIES})

