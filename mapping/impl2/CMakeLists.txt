set(SOURCES
        genDetElemId2SegType.cxx
        genDetElemId2SegType.h
        genPadGroupType.cxx
        genPadSize.cxx
        padGroup.h
        padGroupType.cxx
        padGroupType.h
        padGroupTypeContour.cxx
        padGroupTypeContour.h
        segmentationCImpl2.cxx
        segmentationCreator.cxx
        segmentationCreator.h
        segmentationImpl2.cxx
        segmentationImpl2.h
        )


# We add all segmentation creators by default,
# but the final goal would be to tailor this
# for each executable reaching a given FLP (so it gets only
# the mapping it needs)
foreach (segtype RANGE 20)
    set(SOURCES ${SOURCES} genSegmentationCreatorForSegType${segtype}.cxx)
endforeach ()

add_library(impl2 SHARED ${SOURCES})
target_include_directories(impl2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${MS_GSL_INCLUDE_DIR})
target_link_libraries(impl2 PUBLIC mapping_interface contour Boost::boost)
set_target_properties(impl2 PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_custom_command(TARGET impl2 POST_BUILD
        COMMAND ${CMAKE_SOURCE_DIR}/check_nof_exported_symbols.sh $<TARGET_LINKER_FILE:impl2> 15
        COMMENT "Checking number of exported symbols in the library")

add_library(impl2_static STATIC ${SOURCES})
target_include_directories(impl2_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${MS_GSL_INCLUDE_DIR})
target_link_libraries(impl2_static PUBLIC mapping_interface contour Boost::boost)

add_executable(svgPadGroupType svgPadGroupType.cxx)
target_link_libraries(svgPadGroupType PRIVATE impl2_static)

add_executable(svgSegmentation svgSegmentation.cxx)
target_link_libraries(svgSegmentation PUBLIC impl2_static)
set_target_properties(svgSegmentation PROPERTIES LINK_FLAGS "-Wl,-all_load")

install(TARGETS impl2 impl2_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

include(GenerateExportHeader)
generate_export_header(impl2)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

add_custom_target(impl2_ide SOURCES padSize.h)
