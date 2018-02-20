set(SOURCES
        GenDetElemId2SegType.cxx
        GenDetElemId2SegType.h
        PadGroup.h
        PadGroupType.cxx
        PadGroupType.h
        SegmentationCImpl3.cxx
        SegmentationCreator.cxx
        SegmentationCreator.h
        SegmentationImpl3.cxx
        SegmentationImpl3.h
        )

# We add all segmentation creators by default,
# but the final goal would be to tailor this
# for each executable reaching a given FLP (so it gets only
# the mapping it needs)
foreach (segtype RANGE 20)
    set(SOURCES ${SOURCES} GenSegmentationCreatorForSegType${segtype}.cxx)
endforeach ()

add_library(MCHMappingImpl3 SHARED ${SOURCES})
target_include_directories(MCHMappingImpl3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${MS_GSL_INCLUDE_DIR})
target_link_libraries(MCHMappingImpl3 PUBLIC MCHMappingInterface Boost::boost)
set_target_properties(MCHMappingImpl3 PROPERTIES CXX_VISIBILITY_PRESET hidden)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(MCHMappingImpl3 PRIVATE -fext-numeric-literals)
endif ()
if (APPLE)
    add_custom_command(TARGET MCHMappingImpl3 POST_BUILD
            COMMAND ${CMAKE_SOURCE_DIR}/check_nof_exported_symbols.sh $<TARGET_LINKER_FILE:MCHMappingImpl3> 18
            COMMENT "Checking number of exported symbols in the library")
endif ()

install(TARGETS MCHMappingImpl3
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

include(GenerateExportHeader)
generate_export_header(MCHMappingImpl3)

add_custom_target(MCHMappingImpl3_ide SOURCES PadSize.h)
