
add_executable(testContourCreatorVsAliRoot
        testContourCreatorVsAliRoot.cxx
        commonContour.cxx
        commonContour.h)
target_link_libraries(testContourCreatorVsAliRoot PUBLIC contour MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testContourCreatorVsAliRoot COMMAND testContourCreatorVsAliRoot)

foreach (impl RANGE 3 3 1)

    add_executable(testSegmentationVsAliroot${impl} testFindPadByPosition.cxx testForEachPadInArea.cxx generateTestPoints.cxx generateTestPoints.h mappingFixture.cxx mappingFixture.h testForEachNeighbouringPad.cxx)
    target_link_libraries(testSegmentationVsAliroot${impl} PUBLIC MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework segcontour${impl})
    add_test(NAME testSegmentationVsAliroot${impl} COMMAND testSegmentationVsAliroot${impl})

    add_executable(testSomeGlobalNumbers${impl} testSomeGlobalNumbers.cxx)
    target_link_libraries(testSomeGlobalNumbers${impl} PUBLIC segcontour${impl} MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework)
    add_test(NAME testSomeGlobalNumbers${impl} COMMAND testSomeGlobalNumbers${impl})

endforeach ()

find_package(benchmark)

if (benchmark_FOUND)
    foreach (impl RANGE 3 3 1)

        add_executable(benchSegmentationVsAliRoot${impl} benchNeighbouringPad.cxx benchPadInArea.cxx benchPadByPosition.cxx commonBench.h commonContour.cxx commonBench.cxx generateTestPoints.cxx generateTestPoints.h)
        target_link_libraries(benchSegmentationVsAliRoot${impl} PUBLIC benchmark::benchmark segcontour${impl} MUONmapping MUONgraphics STEERBase ROOT::Core)

    endforeach ()

    add_executable(benchContourVsAliRoot benchContourVsAliRoot.cxx commonBench.h commonContour.cxx commonBench.cxx)
    target_link_libraries(benchContourVsAliRoot PUBLIC benchmark::benchmark contour MUONmapping MUONgraphics STEERBase ROOT::Core)
endif ()
