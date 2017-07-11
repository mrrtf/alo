
set(SOURCE_FILES
        berg2json.h
        bp.cxx
        bp2json.h
        ch.cxx
        ch2json.h
        ddl.cxx
        ddl2json.h
        de.cxx
        de2json.h
        motif.cxx
        motif.h
        motif2json.h
        motifType.cxx
        motifType.h
        motiftype2json.h
        padsize.cxx
        padsize.h
        padsize2json.h
        pcb.h
        pcb2json.h
        pcbnumbers.cxx
        readmapping.cxx
        readmapping.h
        sector2json.h
        seg.cxx
        slat2json.h)
add_library(mchjsoncreator SHARED ${SOURCE_FILES})

add_executable(mch-mapping-convert-to-json mch-mapping-convert-to-json.cxx)

target_link_libraries(mchjsoncreator MUONmapping STEERBase Core)
target_link_libraries(mch-mapping-convert-to-json mchjsoncreator)

target_include_directories(mchjsoncreator PUBLIC ${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

install(TARGETS mch-mapping-convert-to-json RUNTIME DESTINATION bin)

add_subdirectory(test)

