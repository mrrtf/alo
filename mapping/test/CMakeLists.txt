add_executable(testSegmentation testSegmentation.cxx)

target_link_libraries(testSegmentation PRIVATE Boost::unit_test_framework segmentationcontours)

add_test(NAME testSegmentation COMMAND testSegmentation)

