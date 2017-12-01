find_package(benchmark REQUIRED)

add_executable(testContourCreatorVsAliRoot
        testContourCreatorVsAliRoot.cxx)
target_include_directories(testContourCreatorVsAliRoot PUBLIC contour::contour ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
target_link_libraries(testContourCreatorVsAliRoot PUBLIC contour MUONmapping STEERBase Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testContourCreatorVsAliRoot COMMAND testContourCreatorVsAliRoot)

add_executable(testSegmentationVsAliroot testSegmentationVsAliroot.cxx)
target_include_directories(testSegmentationVsAliroot PUBLIC genmap::genmap contour::contour svgwriter::svgwriter ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
target_link_libraries(testSegmentationVsAliroot PUBLIC genmap MUONmapping STEERBase Core MUONgraphics Boost::unit_test_framework svgwriterlib)
add_test(NAME testSegmentationVsAliroot COMMAND testSegmentationVsAliroot)

if(benchmark_FOUND)
    add_executable(benchSegmentation benchSegmentation.cxx)
    target_include_directories(benchSegmentation PRIVATE ${ROOT_INCLUDE_DIR})
    target_link_libraries(benchSegmentation PRIVATE benchmark::benchmark genmap MUONmapping STEERBase Core)
endif()
