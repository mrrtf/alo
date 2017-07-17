find_package(Boost COMPONENTS unit_test_framework)

include_directories(${CMAKE_SOURCE_DIR}/genmap ${BOOST_INCLUDE_DIRS})

add_executable(testgenmap testPadSizes.cxx testSegmentation.cxx testHasIntegerIdWithinGivenRange.cxx)

target_link_libraries(testgenmap PUBLIC Boost::unit_test_framework genmap)

add_test(NAME testgenmap COMMAND testgenmap)
