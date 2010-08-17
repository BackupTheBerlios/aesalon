aux_source_directory(collector CollectorSources)
aux_source_directory(monitor MonitorSources)

add_definitions(-W -Wall)

include_directories(../../static/include/)

add_library(${Module}Collector MODULE ${CollectorSources})
set_property(TARGET ${Module}Collector APPEND PROPERTY COMPILE_DEFINITIONS AesalonCollector)

add_library(${Module}Monitor MODULE ${MonitorSources})
set_property(TARGET ${Module}Monitor APPEND PROPERTY COMPILE_DEFINITIONS AesalonMonitor)

