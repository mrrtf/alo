add_library(genmap2 segmentationFactory.cxx segmentationImpl1.h)
target_link_libraries(genmap2 genmap)

find_package(benchmark)
if (benchmark_FOUND)
    add_subdirectory(bench)
endif ()

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()