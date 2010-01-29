add_definitions(-DDEFAULT_PORT=${DEFAULT_PORT})

aux_source_directory(. SOURCES)

find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)

include(${QT_USE_FILE})

QT4_AUTOMOC(${SOURCES})

include_directories(..)

add_executable(aesalon_gui ${SOURCES})

target_link_libraries(aesalon_gui QtCore QtGui QtNetwork)

install(TARGETS aesalon_gui RUNTIME DESTINATION bin)
