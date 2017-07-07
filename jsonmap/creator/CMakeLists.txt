include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS})

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
        motifType.cxx
        motifType.h
        motiftype2json.h
        padsize.cxx
        padsize.h
        pcb.cxx
        pcb2json.h
        readmapping.cxx
        readmapping.h
        sector2json.h
        seg.cxx
        slat2json.h)
add_library(mchjsoncreator ${SOURCE_FILES})

add_executable(mch-mapping-convert-to-json mch-mapping-convert-to-json.cxx)

target_link_libraries(mchjsoncreator MUONmapping STEERBase Core)
target_link_libraries(mch-mapping-convert-to-json mchjsoncreator)

install(TARGETS mch-mapping-convert-to-json RUNTIME DESTINATION bin)


add_executable(testMapping testMapping.cxx)

target_include_directories(testMapping PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries(testMapping PRIVATE mchjsoncreator)

add_test(NAME testMapping COMMAND testMapping)