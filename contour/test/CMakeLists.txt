find_package(Boost COMPONENTS unit_test_framework)


add_executable(testContour
        testContour.cxx
        testEdge.cxx
        testInterval.cxx
        testPolygon.cxx
        testSegment.cxx
        testSegmentTree.cxx
        testVertex.cxx
        svg.h
        )

target_include_directories(testContour PUBLIC ${CMAKE_SOURCE_DIR}/contour ${BOOST_INCLUDE_DIRS})
target_link_libraries(testContour PUBLIC contour Boost::unit_test_framework)
add_test(NAME testContour COMMAND testContour)

if (AliRoot_FOUND)
    add_executable(testContourVsAliRoot
            testContourVsAliRoot.cxx)
    target_include_directories(testContourVsAliRoot PUBLIC ${CMAKE_SOURCE_DIR}/contour ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
    target_link_libraries(testContourVsAliRoot PUBLIC contour MUONmapping STEERBase Core MUONgraphics Boost::unit_test_framework)
    add_test(NAME testContourVsAliRoot COMMAND testContourVsAliRoot)
endif ()
