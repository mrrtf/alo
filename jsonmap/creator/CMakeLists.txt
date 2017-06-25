include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

set(SOURCE_FILES
        bp.cxx
        bp2json.h
        berg2json.h
        ch.cxx
        ch2json.h
        ddl.cxx
        ddl2json.h
        de.cxx
        de2json.h
        mch-mapping-convert-to-json.cxx
        motif.cxx
        motif2json.h
        pcb.cxx
        pcb2json.h
        sector2json.h
        seg.cxx
        slat2json.h)
add_executable(mch-mapping-convert-to-json ${SOURCE_FILES})

target_link_libraries(mch-mapping-convert-to-json MUONmapping STEERBase Core)

install(TARGETS mch-mapping-convert-to-json RUNTIME DESTINATION bin)

