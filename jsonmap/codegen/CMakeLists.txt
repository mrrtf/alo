
add_executable(mchMappingCodeGenerator mchMappingCodeGenerator.cxx)

target_sources(mchMappingCodeGenerator PUBLIC jsonReader.h jsonReader.cxx)
target_sources(mchMappingCodeGenerator PUBLIC detectionElement.h detectionElement.cxx)
target_sources(mchMappingCodeGenerator PUBLIC codeWriter.h codeWriter.cxx)
target_sources(mchMappingCodeGenerator PUBLIC chamber.h chamber.cxx)
target_sources(mchMappingCodeGenerator PUBLIC motifType.h motifType.cxx)
target_sources(mchMappingCodeGenerator PUBLIC padSize.h padSize.cxx)
target_sources(mchMappingCodeGenerator PUBLIC segmentation.h segmentation.cxx)

target_link_libraries(mchMappingCodeGenerator Boost::program_options)

target_include_directories(mchMappingCodeGenerator PUBLIC ${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

install(TARGETS mchMappingCodeGenerator RUNTIME DESTINATION bin)

