
add_executable(testimpl2 testimpl2.cxx ../genSegmentationCreatorForSegType2.cxx)
target_link_libraries(testimpl2 PUBLIC impl2_static segcontour2 Boost::unit_test_framework)