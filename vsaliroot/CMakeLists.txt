
add_executable(testContourCreatorVsAliRoot
        testContourCreatorVsAliRoot.cxx
        commonContour.cxx
        commonContour.h)
target_link_libraries(testContourCreatorVsAliRoot PUBLIC contour MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testContourCreatorVsAliRoot COMMAND testContourCreatorVsAliRoot)

add_executable(testSegmentationVsAliroot testSegmentationVsAliRoot.cxx)
target_link_libraries(testSegmentationVsAliroot PUBLIC MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework segcontour)
add_test(NAME testSegmentationVsAliroot COMMAND testSegmentationVsAliroot)

add_executable(testSomeGlobalNumbers testSomeGlobalNumbers.cxx)
target_link_libraries(testSomeGlobalNumbers PUBLIC segcontour MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testSomeGlobalNumbers COMMAND testSomeGlobalNumbers)

find_package(benchmark)
if(benchmark_FOUND)
    add_executable(benchSegmentation benchSegmentation.cxx commonBench.h commonContour.cxx commonBench.cxx)
    target_link_libraries(benchSegmentation PRIVATE benchmark::benchmark impl1_static segcontour MUONmapping MUONgraphics STEERBase ROOT::Core)

    add_executable(benchContour benchContour.cxx commonBench.h commonContour.cxx commonBench.cxx)
    target_link_libraries(benchContour PRIVATE benchmark::benchmark contour MUONmapping MUONgraphics STEERBase ROOT::Core)
endif()
