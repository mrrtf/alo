include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

add_executable(mch-mapping-convert-to-json bp.cxx ch.cxx de.cxx ddl.cxx json.cxx mch-mapping-convert-to-json.cxx pcb.cxx seg.cxx)

target_link_libraries(mch-mapping-convert-to-json MUONmapping STEERBase Core)

install(TARGETS mch-mapping-convert-to-json RUNTIME DESTINATION bin)

add_executable(jsonreader jsonreader.cxx)

target_link_libraries(jsonreader)

install(TARGETS jsonreader RUNTIME DESTINATION bin)
