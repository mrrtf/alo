find_package(benchmark)
if (benchmark_FOUND)
    add_executable(benchMotifTypes benchMotifTypes.cxx )
    target_link_libraries(benchMotifTypes impl2_static motiftype1 benchmark::benchmark)
endif ()

