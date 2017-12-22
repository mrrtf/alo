set(SOURCES
        genPadGroupType.cxx
        padGroup.h
        padGroupType.cxx
        padGroupType.h
        segmentationImpl2.cxx
        segmentationImpl2.h
        genPadSize.cxx
        segmentationCImpl2.cxx)
add_library(impl2 SHARED ${SOURCES})
target_include_directories(impl2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(impl2 PUBLIC mapping_interface contour Boost::boost)
set_target_properties(impl2 PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_custom_command(TARGET impl2 POST_BUILD
        COMMAND ${CMAKE_SOURCE_DIR}/check_nof_exported_symbols.sh $<TARGET_LINKER_FILE:impl2> 15
        COMMENT "Checking number of exported symbols in the library")

add_library(impl2_static STATIC ${SOURCES})
target_include_directories(impl2_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(impl2_static PUBLIC mapping_interface contour Boost::boost)

#add_library(segmentation ALIAS impl2)

install(TARGETS impl2 #impl2_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

include(GenerateExportHeader)
generate_export_header(impl2)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()