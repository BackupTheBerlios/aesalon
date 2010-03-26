add_definitions(-DDEFAULT_PORT=${DEFAULT_PORT})
add_definitions(-W -Wall -pedantic -Wno-long-long)

if(PROFILE_BUILD)
    add_definitions(-pg)
endif(PROFILE_BUILD)

aux_source_directory(. DOT_SOURCES)
aux_source_directory(data DATA_SOURCES)
aux_source_directory(main MAIN_SOURCES)
aux_source_directory(session SESSION_SOURCES)
aux_source_directory(visualization VISUALIZATION_SOURCES)
aux_source_directory(storage STORAGE_SOURCES)

set(SOURCES
    ${DOT_SOURCES}
    ${DATA_SOURCES}
    ${MAIN_SOURCES}
    ${SESSION_SOURCES}
    ${VISUALIZATION_SOURCES}
    ${STORAGE_SOURCES}
)

find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)

include(${QT_USE_FILE})

QT4_AUTOMOC(${SOURCES})

include_directories(.)

add_executable(aesalon_gui ${SOURCES})

if(PROFILE_BUILD)
    set_target_properties(aesalon PROPERTIES LINK_FLAGS -pg)
endif(PROFILE_BUILD)

target_link_libraries(aesalon_gui QtCore QtGui QtNetwork)

install(TARGETS aesalon_gui RUNTIME DESTINATION bin)
