find_package(Boost COMPONENTS unit_test_framework)

include_directories(${CMAKE_SOURCE_DIR}/contour ${BOOST_INCLUDE_DIRS})

add_executable(testContour
        testContour.cxx
        testEdge.cxx
        testInterval.cxx
        testPolygon.cxx
        testSegmentTree.cxx
        testVertex.cxx
        )

target_link_libraries(testContour PUBLIC contour Boost::unit_test_framework)

add_test(NAME testContour COMMAND testContour)
