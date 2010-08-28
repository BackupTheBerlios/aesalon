aux_source_directory(collector CollectorSources)
aux_source_directory(monitor MonitorSources)
aux_source_directory(visualizer VisualizerSources)

add_definitions(-W -Wall)

include_directories(../../static/include/)

add_library(${Module}Collector MODULE ${CollectorSources})
set_property(TARGET ${Module}Collector APPEND PROPERTY COMPILE_DEFINITIONS AesalonCollector)

add_library(${Module}Monitor MODULE ${MonitorSources})
set_property(TARGET ${Module}Monitor APPEND PROPERTY COMPILE_DEFINITIONS AesalonMonitor)

add_library(${Module}Visualizer MODULE ${VisualizerSources})
set_property(TARGET ${Module}Visualizer APPEND PROPERTY COMPILE_DEFINITIONS AesalonVisualizer)

add_custom_target(${Module} ALL cp lib${Module}Collector.so ../../../modules/${Module}/collector.so COMMAND cp lib${Module}Monitor.so ../../../modules/${Module}/monitor.so COMMAND cp lib${Module}Visualizer.so ../../../modules/${Module}/visualizer.so)

add_dependencies(${Module} ${Module}Collector ${Module}Monitor ${Module}Visualizer)

