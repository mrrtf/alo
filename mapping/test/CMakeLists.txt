find_package(benchmark)

foreach (impl RANGE 3 3 1)

    add_executable(testSegmentation${impl} testSegmentation.cxx)
    target_link_libraries(testSegmentation${impl} PRIVATE Boost::unit_test_framework segcontour${impl})
    add_test(NAME testSegmentation${impl} COMMAND testSegmentation${impl})
    if (benchmark_FOUND)
        add_executable(benchSegmentation${impl} benchSegmentation.cxx)
        target_link_libraries(benchSegmentation${impl} PRIVATE benchmark::benchmark impl${impl})
    endif ()
endforeach ()

