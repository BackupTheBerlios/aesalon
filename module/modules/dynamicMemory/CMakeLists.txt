aux_source_directory(collector CollectorSources)
aux_source_directory(polisher PolisherSources)
aux_source_directory(renderer RendererSources)

add_definitions(-W -Wall)

include_directories(../../static/include/)

add_library(${Module}Collector MODULE ${CollectorSources})
set_property(TARGET ${Module}Collector APPEND PROPERTY COMPILE_DEFINITIONS AesalonCollector)
target_link_libraries(${Module}Collector dl)

add_library(${Module}Polisher MODULE ${PolisherSources})
set_property(TARGET ${Module}Polisher APPEND PROPERTY COMPILE_DEFINITIONS AesalonMonitor)

add_library(${Module}Renderer MODULE ${RendererSources})
set_property(TARGET ${Module}Renderer APPEND PROPERTY COMPILE_DEFINITIONS AesalonVisualizer)

add_custom_target(${Module}Module ALL cp lib${Module}Collector.so ../../../modules/${Module}/collector.so COMMAND cp lib${Module}Polisher.so ../../../modules/${Module}/polisher.so COMMAND cp lib${Module}Renderer.so ../../../modules/${Module}/renderer.so)

add_dependencies(${Module}Module ${Module}Collector ${Module}Polisher ${Module}Renderer)

