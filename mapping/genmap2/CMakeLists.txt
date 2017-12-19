find_package(benchmark)
if (benchmark_FOUND)
    add_subdirectory(bench)
endif ()

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()