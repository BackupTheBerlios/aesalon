include(build_list)

message(STATUS "Module build list: " ${ModuleList})

foreach(Module ${ModuleList})
    add_subdirectory(${Module} ${Module})
endforeach(Module in ModuleList)

