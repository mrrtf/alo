
add_executable(mch-mapping-generate-code mch-mapping-generate-code.cxx
        jsonReader.h
        writer.h writer.cxx
        chamber.h chamber.cxx
        goImplementation.cxx goImplementation.h
        motifType.h motifType.cxx
        padSize.h padSize.cxx
        cathodeSegmentationCommon.h cathodeSegmentationCommon.cxx
        cathodeSegmentation1.h cathodeSegmentation1.cxx
        cathodeSegmentation2.h cathodeSegmentation2.cxx
        motifPosition.h motifPosition.cxx
        motifPosition1.h motifPosition1.cxx
        motifPosition2.h motifPosition2.cxx
        padGroupType.h padGroupType.cxx
        padGroup.h padGroup.cxx
        report.cxx report.h)

target_link_libraries(mch-mapping-generate-code PUBLIC Boost::program_options RapidJSON::RapidJSON)

add_executable(testBerg testBerg.cxx berg.cxx)
add_test(NAME testBerg COMMAND testBerg)

target_link_libraries(testBerg PUBLIC Boost::unit_test_framework RapidJSON::RapidJSON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../data/run2/bergs.json
        ${CMAKE_CURRENT_BINARY_DIR}/bergs-run2.json COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../data/run3/bergs.json
        ${CMAKE_CURRENT_BINARY_DIR}/bergs-run3.json COPYONLY)

install(TARGETS mch-mapping-generate-code RUNTIME DESTINATION bin)

