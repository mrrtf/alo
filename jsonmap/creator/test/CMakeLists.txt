include_directories(${CMAKE_SOURCE_DIR}/jsonmap/creator ${BOOST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

add_library(mapping SHARED mapping.cxx mapping.h segnumbers.h segnumbers.cxx pcbnumbers.h)

add_executable(testMapping testMapping.cxx testPCB testPads testMotifType.cxx testMotif.cxx testPadSize.cxx testSegmentations.cxx
        testMotifPosition.cxx)

target_link_libraries(mapping PUBLIC mchjsoncreator Boost::unit_test_framework)

target_link_libraries(testMapping PUBLIC mapping)

add_test(NAME testMapping COMMAND testMapping)
