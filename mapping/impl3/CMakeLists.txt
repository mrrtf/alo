set(SOURCES
        genDetElemId2SegType.cxx
        genDetElemId2SegType.h
        padGroup.h
        padGroupType.cxx
        padGroupType.h
        segmentationCImpl3.cxx
        segmentationCreator.cxx
        segmentationCreator.h
        segmentationImpl3.cxx
        segmentationImpl3.h
        )

# We add all segmentation creators by default,
# but the final goal would be to tailor this
# for each executable reaching a given FLP (so it gets only
# the mapping it needs)
foreach (segtype RANGE 20)
    set(SOURCES ${SOURCES} genSegmentationCreatorForSegType${segtype}.cxx)
endforeach ()

add_library(impl3 SHARED ${SOURCES})
target_include_directories(impl3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${MS_GSL_INCLUDE_DIR})
target_link_libraries(impl3 PUBLIC mapping_interface Boost::boost)
set_target_properties(impl3 PROPERTIES CXX_VISIBILITY_PRESET hidden)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(impl3 PRIVATE -fext-numeric-literals)
endif ()
if (APPLE)
    add_custom_command(TARGET impl3 POST_BUILD
            COMMAND ${CMAKE_SOURCE_DIR}/check_nof_exported_symbols.sh $<TARGET_LINKER_FILE:impl3> 18
            COMMENT "Checking number of exported symbols in the library")
endif ()

install(TARGETS impl3
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

include(GenerateExportHeader)
generate_export_header(impl3)

add_custom_target(impl3_ide SOURCES padSize.h)
