add_executable(testgenmap testPadSizes.cxx testSegmentation.cxx testMotifType.cxx testDetectionElement.cxx)

target_link_libraries(testgenmap PRIVATE Boost::unit_test_framework genmap)

add_test(NAME testgenmap COMMAND testgenmap)

