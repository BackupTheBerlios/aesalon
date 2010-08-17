include(build_list)

message(STATUS "Module build list: " ${ModuleList})

foreach(Module ${ModuleList})
    add_custom_target(${Module} ALL COMMAND /usr/bin/make -C ${Module} WORKING_DIRECTORY module/modules/)
endforeach(Module in ModuleList)

