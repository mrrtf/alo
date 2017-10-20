include_directories(${CMAKE_SOURCE_DIR}/genmap ${BOOST_INCLUDE_DIRS})

add_executable(testgenmap testPadSizes.cxx testSegmentation.cxx testMotifType.cxx)

target_link_libraries(testgenmap PUBLIC Boost::unit_test_framework genmap)

add_test(NAME testgenmap COMMAND testgenmap)

