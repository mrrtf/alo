
add_executable(mchMappingCodeGenerator mchMappingCodeGenerator.cxx)

target_sources(mchMappingCodeGenerator PUBLIC jsonReader.h jsonReader.cxx)
target_sources(mchMappingCodeGenerator PUBLIC codeWriter.h codeWriter.cxx)
target_sources(mchMappingCodeGenerator PUBLIC chamber.h chamber.cxx)
target_sources(mchMappingCodeGenerator PUBLIC motifType.h motifType.cxx)
target_sources(mchMappingCodeGenerator PUBLIC padSize.h padSize.cxx)
target_sources(mchMappingCodeGenerator PUBLIC segmentation.h segmentationCInterface.cxx)
target_sources(mchMappingCodeGenerator PUBLIC motifPosition.h motifPosition.cxx)
target_sources(mchMappingCodeGenerator PUBLIC feePadGroup.h feePadGroup.cxx)

target_link_libraries(mchMappingCodeGenerator Boost::program_options RapidJSON)

install(TARGETS mchMappingCodeGenerator RUNTIME DESTINATION bin)

