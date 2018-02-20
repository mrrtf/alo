find_package(benchmark)

foreach (impl RANGE 3 3 1)

    add_executable(test_Segmentation${impl} test_Segmentation.cxx)
    target_link_libraries(test_Segmentation${impl} PRIVATE Boost::unit_test_framework MCHMappingSegContour${impl})
    add_test(NAME test_Segmentation${impl} COMMAND test_Segmentation${impl})
    if (benchmark_FOUND)
        add_executable(bench_Segmentation${impl} bench_Segmentation.cxx)
        target_link_libraries(bench_Segmentation${impl} PRIVATE benchmark::benchmark MCHMappingImpl${impl})
    endif ()
endforeach ()

