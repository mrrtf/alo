
add_executable(testContourCreatorVsAliRoot
        testContourCreatorVsAliRoot.cxx
        commonContour.cxx
        commonContour.h)
target_link_libraries(testContourCreatorVsAliRoot PUBLIC contour MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testContourCreatorVsAliRoot COMMAND testContourCreatorVsAliRoot)

add_executable(testSegmentationVsAliroot testSegmentationVsAliroot.cxx)
target_link_libraries(testSegmentationVsAliroot PUBLIC genmap MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework svgwriterlib segmentationcontours)
add_test(NAME testSegmentationVsAliroot COMMAND testSegmentationVsAliroot)

add_executable(testSegmentationVsAliroot2 testSegmentationVsAliroot2.cxx)
target_link_libraries(testSegmentationVsAliroot2 PUBLIC MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework svgwriterlib segmentationcontours)
add_test(NAME testSegmentationVsAliroot2 COMMAND testSegmentationVsAliroot2)

add_executable(testSomeGlobalNumbers testSomeGlobalNumbers.cxx)
target_link_libraries(testSomeGlobalNumbers PUBLIC genmap MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework svgwriterlib)
add_test(NAME testSomeGlobalNumbers COMMAND testSomeGlobalNumbers)

find_package(benchmark)
if(benchmark_FOUND)
    add_executable(benchSegmentation benchSegmentation.cxx commonBench.h commonContour.cxx commonBench.cxx)
    target_link_libraries(benchSegmentation PRIVATE benchmark::benchmark genmap segmentationcontours MUONmapping MUONgraphics STEERBase ROOT::Core)

    add_executable(benchContour benchContour.cxx commonBench.h commonContour.cxx commonBench.cxx)
    target_link_libraries(benchContour PRIVATE benchmark::benchmark contour MUONmapping MUONgraphics STEERBase ROOT::Core)
endif()
