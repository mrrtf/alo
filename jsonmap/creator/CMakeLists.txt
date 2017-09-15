
set(SOURCE_FILES
        berg2json.h
        bp.cxx
        bp2json.h
        ch.cxx
        ch2json.h
        contour.cxx
        contour.h
        ddl.cxx
        ddl2json.h
        de.cxx
        de2json.h
        edge.h
        interval.h
        json.h
        legacyseg2json.h
        motif.cxx
        motif.h
        motif2json.h
        motifPosition.cxx
        motifPosition.h
        motifType.cxx
        motifType.h
        motiftype2json.h
        padsize.cxx
        padsize.h
        padsize2json.h
        pcb.h
        pcb2json.h
        pcbnumbers.cxx
        polygon.cxx
        polygon.h
        readmapping.cxx
        readmapping.h
        sector2json.h
        seg.cxx
        seg2json.h
        segmentTree.cxx
        segmentTree.h
        slat2json.h
        vertex.h
        )
add_library(mchjsoncreator STATIC ${SOURCE_FILES})

add_executable(mch-mapping-convert-to-json mch-mapping-convert-to-json.cxx)

target_link_libraries(mchjsoncreator MUONmapping STEERBase Core MUONgraphics)
target_link_libraries(mch-mapping-convert-to-json mchjsoncreator)

target_include_directories(mchjsoncreator PUBLIC ${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

install(TARGETS mch-mapping-convert-to-json RUNTIME DESTINATION bin)

add_subdirectory(test)

