add_executable(testBGI testBGI.cxx)

target_link_libraries(testBGI PRIVATE Boost::unit_test_framework genmap2)

add_test(NAME testBGI COMMAND testBGI)
