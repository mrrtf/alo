include_directories(${CMAKE_SOURCE_DIR}/dhlt ${ROOT_INCLUDE_DIR})

add_executable(dhlt-generate-binmapfile dhlt-generate-binmapfile.cxx)
target_link_libraries(dhlt-generate-binmapfile dhlt AliHLTMUON HLTbase MUONmapping CDB MUONbase MUONrec)

install(TARGETS dhlt-generate-binmapfile RUNTIME DESTINATION bin)

add_executable(mch-mapping-convert-to-json json.cxx bp2json.cxx ch2json.cxx ddl2json.cxx de2json.cxx
    mch-mapping-convert-to-json.cxx seg2json.cxx slat2json.cxx pcb2json.cxx)
target_link_libraries(mch-mapping-convert-to-json MUONmapping STEERBase Core)

install(TARGETS mch-mapping-convert-to-json RUNTIME DESTINATION bin)
