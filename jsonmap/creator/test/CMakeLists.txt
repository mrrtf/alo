add_library(jsonmap-creator-test SHARED mapping.cxx mapping.h  pcbnumbers.h)

add_executable(testMapping testMapping.cxx testPCB testPads testMotifType.cxx testMotif.cxx testPadSize.cxx testSegmentations.cxx
        testMotifPosition.cxx)

target_link_libraries(jsonmap-creator-test PRIVATE mchjsoncreator Boost::unit_test_framework)

target_link_libraries(testMapping PRIVATE jsonmap-creator-test Boost::unit_test_framework mchjsoncreator)

add_test(NAME testMapping COMMAND testMapping)
