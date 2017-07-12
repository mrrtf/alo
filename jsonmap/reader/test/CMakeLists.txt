find_package(Boost COMPONENTS unit_test_framework)

include_directories(${CMAKE_SOURCE_DIR}/jsonmap/reader ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

add_executable(testGeneratedMapping testPadSizes.cxx)

target_link_libraries(testGeneratedMapping PUBLIC Boost::unit_test_framework mchGeneratedMapping)

add_test(NAME testGeneratedMapping COMMAND testGeneratedMapping)
