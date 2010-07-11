include(build_list)

add_definitions(-W -Wall)

foreach(test ${testList})
    add_executable(${test} ${test}.c)
endforeach(test)
