add_definitions(-W -Wall -D_GNU_SOURCE)
add_definitions(-DAesalonCollector)

aux_source_directory(. CollectorSources)

add_library(collector MODULE ${CollectorSources})

target_link_libraries(collector rt)
