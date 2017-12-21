
foreach (impl RANGE 1 2 1)

    add_executable(testSegmentation${impl} testSegmentation.cxx)
    target_link_libraries(testSegmentation${impl} PRIVATE Boost::unit_test_framework impl${impl})
    add_test(NAME testSegmentation${impl} COMMAND testSegmentation${impl})

endforeach ()

