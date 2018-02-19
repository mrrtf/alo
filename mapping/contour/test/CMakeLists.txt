add_executable(testContour
        testBBox.cxx
        testContour.cxx
        testContourCreator.cxx
        testEdge.cxx
        testInterval.cxx
        testPolygon.cxx
        testSegmentTree.cxx
        testVertex.cxx
        )

target_include_directories(testContour PUBLIC contour ${BOOST_INCLUDE_DIRS})
target_link_libraries(testContour PUBLIC contour Boost::unit_test_framework)
add_test(NAME testContour COMMAND testContour)

