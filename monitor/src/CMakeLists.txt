add_definitions(-std=c++0x -W -Wall -ansi -pedantic -Wno-unused-parameter -Wno-long-long -DDEFAULT_PORT=${DEFAULT_PORT} -DLIBC_PATH="${LIBC_PATH}")
add_definitions(-DAESALON_MAJOR_VERSION="${AESALON_MAJOR_VERSION}" -DAESALON_MINOR_VERSION="${AESALON_MINOR_VERSION}" -DAESALON_PATCHLEVEL="${AESALON_PATCHLEVEL}" -DAESALON_PLATFORM=${AESALON_PLATFORM})
add_definitions(-DAESALON_PLATFORM_x86=0 -DAESALON_PLATFORM_x86_64=1)

add_definitions(-DSYS_CONFIG_PATH="${SYS_CONFIG_PATH}" -DUSER_CONFIG_PATH="${USER_CONFIG_PATH}")

if(PROFILE_BUILD)
    add_definitions(-pg)
endif(PROFILE_BUILD)

if(DEVELOPMENT_BUILD)
    add_definitions(-DAESALON_DEV_BUILD)
endif(DEVELOPMENT_BUILD)

include_directories(.)

aux_source_directory(. DOT_SOURCES)
aux_source_directory(asm ASM_SOURCES)
aux_source_directory(elf ELF_SOURCES)
aux_source_directory(event EVENT_SOURCES)
aux_source_directory(exception EXCEPTION_SOURCES)
aux_source_directory(misc MISC_SOURCES)
aux_source_directory(ptrace PTRACE_SOURCES)
aux_source_directory(taint TAINT_SOURCES)
aux_source_directory(tcp TCP_SOURCES)
aux_source_directory(tracker TRACKER_SOURCES)

set(SOURCES
    ${DOT_SOURCES}
    ${ASM_SOURCES}
    ${ELF_SOURCES}
    ${EVENT_SOURCES}
    ${EXCEPTION_SOURCES}
    ${MISC_SOURCES}
    ${PTRACE_SOURCES}
    ${TAINT_SOURCES}
    ${TCP_SOURCES}
    ${TRACKER_SOURCES}
)

add_executable(aesalon ${SOURCES})

target_link_libraries(aesalon rt)

install(TARGETS aesalon RUNTIME DESTINATION bin)

if(PROFILE_BUILD)
    set_target_properties(aesalon PROPERTIES LINK_FLAGS -pg)
endif(PROFILE_BUILD)

add_subdirectory(overload)
