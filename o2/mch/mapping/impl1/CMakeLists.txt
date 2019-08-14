set(SOURCES
        genMotifType.cxx
        genMotifPosition.cxx
        genMotifPosition.h
        genPadSize.cxx
        motifPosition.h
        motifPositionContours.cxx
        motifPositionContours.h
        motifType.h
        pad.h
        padSize.h
        segmentationFactory.cxx
        segmentationImpl1.h
        zone.h
        segmentationFactory.h
        segmentationImpl1.cxx)

add_library(motiftype1 SHARED motifType.h genMotifType.cxx)
target_include_directories(motiftype1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(impl1 SHARED ${SOURCES})
target_include_directories(impl1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(impl1 PUBLIC mapping_interface contour Boost::boost)
set_target_properties(impl1 PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_custom_command(TARGET impl1 POST_BUILD
        COMMAND ${CMAKE_SOURCE_DIR}/check_nof_exported_symbols.sh $<TARGET_LINKER_FILE:impl1> 16
        COMMENT "Checking number of exported symbols in the library")

add_library(impl1_static STATIC ${SOURCES})
target_include_directories(impl1_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(impl1_static PUBLIC O2::mapping_interface O2::contour Boost::boost)

install(TARGETS impl1 impl1_static motiftype1
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

include(GenerateExportHeader)
generate_export_header(impl1)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

add_custom_target(impl1_ide SOURCES genSegmentationFactory.cxx)
