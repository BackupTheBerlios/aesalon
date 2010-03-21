add_definitions(-DDEFAULT_PORT=${DEFAULT_PORT})

add_custom_target("gui_doc" /usr/bin/doxygen)

add_subdirectory(src)
