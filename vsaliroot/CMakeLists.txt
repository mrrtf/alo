
add_executable(testContourCreatorVsAliRoot
        testContourCreatorVsAliRoot.cxx
        commonContour.cxx
        commonContour.h)
#target_include_directories(testContourCreatorVsAliRoot PUBLIC contour ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
target_link_libraries(testContourCreatorVsAliRoot PUBLIC contour MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testContourCreatorVsAliRoot COMMAND testContourCreatorVsAliRoot)

add_executable(testSegmentationVsAliroot testSegmentationVsAliroot.cxx)
#target_include_directories(testSegmentationVsAliroot PUBLIC genmap::genmap contour::contour svgwriter::svgwriter ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
target_link_libraries(testSegmentationVsAliroot PUBLIC genmap MUONmapping STEERBase ROOT::Core MUONgraphics Boost::unit_test_framework svgwriterlib)
add_test(NAME testSegmentationVsAliroot COMMAND testSegmentationVsAliroot)

find_package(benchmark)
if(benchmark_FOUND)
    add_executable(benchSegmentation benchSegmentation.cxx commonBench.h commonContour.cxx commonBench.cxx)
    target_link_libraries(benchSegmentation PRIVATE benchmark::benchmark genmap MUONmapping MUONgraphics STEERBase ROOT::Core)

    add_executable(benchContour benchContour.cxx commonBench.h commonContour.cxx commonBench.cxx)
    target_link_libraries(benchContour PRIVATE benchmark::benchmark contour MUONmapping MUONgraphics STEERBase ROOT::Core)
endif()
