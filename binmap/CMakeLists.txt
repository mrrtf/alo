include_directories(${CMAKE_SOURCE_DIR}/dhlt ${ROOT_INCLUDE_DIR})

add_executable(dhlt-generate-binmapfile dhlt-generate-binmapfile.cxx)
target_link_libraries(dhlt-generate-binmapfile dhlt AliHLTMUON HLTbase MUONmapping CDB MUONbase MUONrec)

install(TARGETS dhlt-generate-binmapfile RUNTIME DESTINATION bin)

