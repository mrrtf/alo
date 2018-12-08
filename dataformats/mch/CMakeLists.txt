set_property(SOURCE Run3_generated.h PROPERTY GENERATED TRUE)
set(GENE ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(fbs_includes
        DEPENDS ${GENE}/Run3_generated.h
        ${GENE}/Run2_generated.h)

add_custom_command(OUTPUT ${GENE}/Run3_generated.h
        COMMAND ${FLATBUFFERS_BINARY_FLATC} -o ${GENE}
        --cpp ${CMAKE_CURRENT_SOURCE_DIR}/Run3.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Run3.fbs)

add_custom_command(OUTPUT ${GENE}/Run2_generated.h
        COMMAND ${FLATBUFFERS_BINARY_FLATC} -o ${GENE}
        --cpp ${CMAKE_CURRENT_SOURCE_DIR}/Run2.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Run2.fbs)

add_library(MCHDataFormat INTERFACE)
target_include_directories(MCHDataFormat INTERFACE . ${GENE})
target_link_libraries(MCHDataFormat INTERFACE flatbuffers::flatbuffers)
add_dependencies(MCHDataFormat fbs_includes)

install(DIRECTORY . DESTINATION include FILES_MATCHING PATTERN "*.h")

add_executable(mch-dump-run2 mch-dump-run2.cxx DumpRun2.cxx Run2FB2YAML.cxx)
target_link_libraries(mch-dump-run2 MCHDataFormat Boost::program_options yaml-cpp)

add_executable(mch-yaml-test mch-yaml-test.cxx Run2FB2YAML.cxx)
target_link_libraries(mch-yaml-test MCHDataFormat yaml-cpp)

install(TARGETS mch-dump-run2 mch-yaml-test RUNTIME DESTINATION bin)
