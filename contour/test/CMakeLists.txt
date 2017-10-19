find_package(Boost COMPONENTS unit_test_framework)


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

if (AliRoot_FOUND)
    add_executable(testContourVsAliRoot
            testContourCreatorVsAliRoot.cxx)
    target_include_directories(testContourVsAliRoot PUBLIC ${CMAKE_SOURCE_DIR}/contour ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
    target_link_libraries(testContourVsAliRoot PUBLIC contour MUONmapping STEERBase Core MUONgraphics Boost::unit_test_framework)
    add_test(NAME testContourVsAliRoot COMMAND testContourVsAliRoot)
endif ()
