add_executable(mch-convert-esd 
        mch-convert-esd.cxx 
        FileCreation.cxx
        MappingHelper.cxx
        ConvertESD.cxx
        GetESDClusters.cxx
        SegmentationPair.cxx
        )

target_link_libraries(mch-convert-esd PUBLIC flatbuffers::flatbuffers ROOT::RIO AliRoot::ESD MCHDataFormat Boost::program_options AliRoot::MUONmapping)
install(TARGETS mch-convert-esd RUNTIME DESTINATION bin)

add_executable(test_FileCreation test_FileCreation.cxx FileCreation.cxx)
target_link_libraries(test_FileCreation PRIVATE Boost::unit_test_framework Boost::filesystem)
add_test(NAME test_FileCreation COMMAND test_FileCreation)
install(TARGETS test_FileCreation RUNTIME DESTINATION bin)
