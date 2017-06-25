
find_package(Boost COMPONENTS program_options)

add_executable(mchMappingReader mchMappingReader.cxx)

target_sources(mchMappingReader PUBLIC jsonReader.h jsonReader.cxx)
target_sources(mchMappingReader PUBLIC detectionElement.h detectionElement.cxx)
target_sources(mchMappingReader PUBLIC codeWriter.h codeWriter.cxx)
target_sources(mchMappingReader PUBLIC chamber.h chamber.cxx)

target_link_libraries(mchMappingReader Boost::program_options)

target_include_directories(mchMappingReader PUBLIC ${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})
