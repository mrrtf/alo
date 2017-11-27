add_executable(testContour
        testBBox.cxx
        testInterval.cxx
        testEdge.cxx
        testPolygon.cxx
        testSegmentTree.cxx
        testVertex.cxx
        svg.h
        testContour.cxx
        testContourCreator.cxx testBBox.cxx)

target_include_directories(testContour PUBLIC ${CMAKE_SOURCE_DIR}/contour ${BOOST_INCLUDE_DIRS})
target_link_libraries(testContour PUBLIC contour Boost::unit_test_framework)
add_test(NAME testContour COMMAND testContour)

