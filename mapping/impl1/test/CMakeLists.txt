add_executable(testimpl1 testPadSizes.cxx testMotifType.cxx testDetectionElement.cxx testMotifPositions.cxx)

target_link_libraries(testimpl1 PRIVATE Boost::unit_test_framework impl1_static)

add_test(NAME testimpl1 COMMAND testimpl1)

find_package(benchmark)

if (benchmark_FOUND)

    add_executable(benchPadAccess benchPadAccess.cxx)
    target_link_libraries(benchPadAccess benchmark::benchmark)

    add_executable(benchSegmentationFactory benchSegmentationFactory.cxx)
    target_link_libraries(benchSegmentationFactory benchmark::benchmark impl1_static)

endif ()

