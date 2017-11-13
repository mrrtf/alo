add_executable(testContourCreatorVsAliRoot
        testContourCreatorVsAliRoot.cxx)
target_include_directories(testContourCreatorVsAliRoot PUBLIC ${CMAKE_SOURCE_DIR}/contour ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
target_link_libraries(testContourCreatorVsAliRoot PUBLIC contour MUONmapping STEERBase Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testContourCreatorVsAliRoot COMMAND testContourCreatorVsAliRoot)

add_executable(testSegmentationVsAliroot testSegmentationVsAliroot.cxx)
target_include_directories(testSegmentationVsAliroot PUBLIC ${CMAKE_SOURCE_DIR}/genmap ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR})
target_link_libraries(testSegmentationVsAliroot PUBLIC genmap MUONmapping STEERBase Core MUONgraphics Boost::unit_test_framework)
add_test(NAME testSegmentationVsAliroot COMMAND testSegmentationVsAliroot)
