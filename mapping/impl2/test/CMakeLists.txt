#[[add_executable(testBGI testBGI.cxx)
target_link_libraries(testBGI PRIVATE Boost::unit_test_framework segcontour)
add_test(NAME testBGI COMMAND testBGI)]]

find_package(benchmark)
if (benchmark_FOUND)
#    add_executable(benchBGI benchBGI.cxx)
#    target_link_libraries(benchBGI PRIVATE benchmark::benchmark segcontour)
    add_executable(benchMotifTypes benchMotifTypes.cxx )
    target_link_libraries(benchMotifTypes impl2_static motiftype1 benchmark::benchmark)
endif ()

