include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

add_executable(mch-mapping-convert-to-json json.cxx bp2json.cxx ch2json.cxx ddl2json.cxx de2json.cxx
    mch-mapping-convert-to-json.cxx seg2json.cxx slat2json.cxx pcb2json.cxx motif2json.cxx berg2json.cxx)

target_link_libraries(mch-mapping-convert-to-json MUONmapping STEERBase Core)

install(TARGETS mch-mapping-convert-to-json RUNTIME DESTINATION bin)

add_executable(jsonreader jsonreader.cxx)

target_link_libraries(jsonreader)

install(TARGETS jsonreader RUNTIME DESTINATION bin)
