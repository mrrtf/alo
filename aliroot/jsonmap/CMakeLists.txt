add_library(mchjsoncreator
            SHARED
            berg2json.h
            bp.cxx
            bp2json.h
            ch.cxx
            ch2json.h
            ddl.cxx
            ddl2json.h
            de.cxx
            de2json.h
            generateTestFiles.cxx
            generateTestFiles.h
            json.h
            legacyseg2json.h
            motif.cxx
            motif.h
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
            readmapping.cxx
            readmapping.h
            sector2json.h
            seg.cxx
            seg2json.h
            segnumbers.h
            segnumbers.cxx
            slat2json.h)

target_link_libraries(mchjsoncreator PUBLIC AliRoot::MUONmapping RapidJSON)
target_include_directories(mchjsoncreator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(mch-mapping-convert-to-json mch-mapping-convert-to-json.cxx)
target_link_libraries(mch-mapping-convert-to-json mchjsoncreator Boost::program_options)

add_executable(mid-generate-test-files mid-generate-test-files.cxx)
target_link_libraries(mid-generate-test-files mchjsoncreator AliRoot::MUONgeometry Boost::program_options)

install(TARGETS mch-mapping-convert-to-json mchjsoncreator mid-generate-test-files
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
