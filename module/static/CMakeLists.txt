add_definitions(-W -Wall -D_GNU_SOURCE)
add_definitions(-DAesalonCollector)

include_directories("./include")

aux_source_directory(src/collector CollectorSources)

add_library(collector MODULE ${CollectorSources})

target_link_libraries(collector rt)
