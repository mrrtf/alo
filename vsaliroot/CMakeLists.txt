add_executable(testContourVsAliRoot
        testContourCreatorVsAliRoot.cxx)
target_include_directories(testContourVsAliRoot PUBLIC ../contour ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
target_link_libraries(testContourVsAliRoot PUBLIC contour MUONmapping STEERBase Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testContourVsAliRoot COMMAND testContourVsAliRoot)

add_executable(testSegmentationVsAliroot testSegmentationVsAliroot.cxx)
target_include_directories(testSegmentationVsAliroot PUBLIC ../genmap ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
target_link_libraries(testSegmentationVsAliroot PUBLIC contour MUONmapping STEERBase Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testSegmentationVsAliroot COMMAND testSegmentationVsAliroot)
