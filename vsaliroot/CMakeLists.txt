
add_executable(test_ContourCreatorVsAliRoot
        test_ContourCreatorVsAliRoot.cxx
        CommonContour.cxx
        CommonContour.h)
target_link_libraries(test_ContourCreatorVsAliRoot PUBLIC o2::MCHContour AliRoot::MUONgraphics Boost::unit_test_framework)
add_test(NAME test_ContourCreatorVsAliRoot COMMAND test_ContourCreatorVsAliRoot)

foreach (impl RANGE 3 3 1)

    add_executable(test_SegmentationVsAliroot${impl} test_FindPadByPosition.cxx test_ForEachPadInArea.cxx GenerateTestPoints.cxx GenerateTestPoints.h MappingFixture.cxx MappingFixture.h test_ForEachNeighbouringPad.cxx)
    target_link_libraries(test_SegmentationVsAliroot${impl} PUBLIC AliRoot::MUONgraphics Boost::unit_test_framework o2::MCHMappingSegContour${impl})
    add_test(NAME test_SegmentationVsAliroot${impl} COMMAND test_SegmentationVsAliroot${impl})

    add_executable(test_SomeGlobalNumbers${impl} test_SomeGlobalNumbers.cxx)
    target_link_libraries(test_SomeGlobalNumbers${impl} PUBLIC o2::MCHMappingSegContour${impl} AliRoot::MUONgraphics Boost::unit_test_framework)
    add_test(NAME test_SomeGlobalNumbers${impl} COMMAND test_SomeGlobalNumbers${impl})

endforeach ()

add_executable(test_MIDSegmentation test_MIDSegmentation.cxx)
target_link_libraries(test_MIDSegmentation PUBLIC AliRoot::MUONgraphics AliRoot::MUONmapping Boost::unit_test_framework o2::MIDBase)
add_test(NAME test_MIDSegmentation COMMAND test_MIDSegmentation)

add_executable(test_MIDGeomTransformer test_MIDGeomTransformer.cxx)
target_link_libraries(test_MIDGeomTransformer PUBLIC AliRoot::MUONgeometry Boost::unit_test_framework o2::MIDBase)
add_test(NAME test_MIDGeomTransformer COMMAND test_MIDGeomTransformer)

find_package(benchmark)

if (benchmark_FOUND)
    foreach (impl RANGE 3 3 1)

        add_executable(bench_SegmentationVsAliRoot${impl} bench_NeighbouringPad.cxx bench_PadInArea.cxx bench_PadByPosition.cxx CommonBench.h CommonContour.cxx CommonBench.cxx GenerateTestPoints.cxx GenerateTestPoints.h)
        target_link_libraries(bench_SegmentationVsAliRoot${impl} PUBLIC benchmark::benchmark o2::MCHMappingSegContour${impl} AliRoot::MUONgraphics)

    endforeach ()

    add_executable(bench_ContourVsAliRoot bench_ContourVsAliRoot.cxx CommonBench.h CommonContour.cxx CommonBench.cxx)
    target_link_libraries(bench_ContourVsAliRoot PUBLIC benchmark::benchmark o2::MCHContour AliRoot::MUONgraphics)

    add_executable(bench_MIDSegmentation bench_MIDSegmentation.cxx CommonBench.h CommonBench.cxx)
    target_link_libraries(bench_MIDSegmentation PUBLIC benchmark::benchmark o2::MIDBase AliRoot::MUONmapping AliRoot::MUONgraphics)
endif ()
