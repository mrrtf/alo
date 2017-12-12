add_executable(testJsonMapCreator testMapping.cxx testPCB testPads testMotifType.cxx testMotif.cxx testPadSize.cxx testSegmentations.cxx
        testMotifPosition.cxx mapping.cxx mapping.h pcbnumbers.h)

target_link_libraries(testJsonMapCreator PRIVATE mchjsoncreator Boost::unit_test_framework)

add_test(NAME testJsonMapCreator COMMAND testJsonMapCreator)
