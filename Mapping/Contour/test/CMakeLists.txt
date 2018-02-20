add_executable(test_Contour
        test_BBox.cxx
        test_Contour.cxx
        test_ContourCreator.cxx
        test_Edge.cxx
        test_Interval.cxx
        test_Polygon.cxx
        test_SegmentTree.cxx
        test_Vertex.cxx
        )

target_include_directories(test_Contour PUBLIC contour ${BOOST_INCLUDE_DIRS})
target_link_libraries(test_Contour PUBLIC contour Boost::unit_test_framework)
add_test(NAME test_Contour COMMAND test_Contour)

