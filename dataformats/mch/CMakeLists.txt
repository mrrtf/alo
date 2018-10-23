set_property(SOURCE Digit_generated.h PROPERTY GENERATED TRUE)
set(GENE ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(fbs_includes
                  DEPENDS ${GENE}/Digit_generated.h ${GENE}/Cluster_generated.h
                          ${GENE}/Run2_generated.h)

add_custom_command(OUTPUT ${GENE}/Digit_generated.h
                   COMMAND ${FLATBUFFERS_BINARY_FLATC} -o ${GENE}
                           --cpp ${CMAKE_CURRENT_SOURCE_DIR}/Digit.fbs
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Digit.fbs)

add_custom_command(OUTPUT ${GENE}/Cluster_generated.h
                   COMMAND ${FLATBUFFERS_BINARY_FLATC} -o ${GENE}
                           --cpp ${CMAKE_CURRENT_SOURCE_DIR}/Cluster.fbs
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Cluster.fbs)

add_custom_command(OUTPUT ${GENE}/Run2_generated.h
                   COMMAND ${FLATBUFFERS_BINARY_FLATC} -o ${GENE}
                           --cpp ${CMAKE_CURRENT_SOURCE_DIR}/Run2.fbs
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Run2.fbs)

add_library(MCHDataFormat INTERFACE)
target_include_directories(MCHDataFormat INTERFACE . ${GENE})
target_link_libraries(MCHDataFormat INTERFACE flatbuffers::flatbuffers)
add_dependencies(MCHDataFormat fbs_includes)

install(DIRECTORY . DESTINATION include FILES_MATCHING PATTERN "*.h")

add_executable(mch-dump-run2 mch-dump-run2.cxx DumpRun2Clusters.cxx)
target_link_libraries(mch-dump-run2 MCHDataFormat Boost::program_options)

install(TARGETS mch-dump-run2 RUNTIME DESTINATION bin)
