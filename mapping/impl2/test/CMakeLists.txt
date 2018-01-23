#[[add_executable(testBGI testBGI.cxx)
target_link_libraries(testBGI PRIVATE Boost::unit_test_framework segcontour)
add_test(NAME testBGI COMMAND testBGI)]]

add_executable(testSegmentationImpl2 testSegmentation.cxx)
target_link_libraries(testSegmentationImpl2 PRIVATE Boost::unit_test_framework impl2_static)
add_test(NAME testSegmentationImpl2 COMMAND testSegmentationImpl2)
set_target_properties(testSegmentationImpl2 PROPERTIES LINK_FLAGS "-Wl,-all_load")

find_package(benchmark)
if (benchmark_FOUND)
#    add_executable(benchBGI benchBGI.cxx)
#    target_link_libraries(benchBGI PRIVATE benchmark::benchmark segcontour)
    add_executable(benchMotifTypes benchMotifTypes.cxx )
    target_link_libraries(benchMotifTypes impl2_static motiftype1 benchmark::benchmark)
endif ()

