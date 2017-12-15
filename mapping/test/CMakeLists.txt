add_executable(testmapping testSegmentation.cxx )

target_link_libraries(testmapping PRIVATE Boost::unit_test_framework genmap genmap2)

add_test(NAME testmapping COMMAND testmapping)

