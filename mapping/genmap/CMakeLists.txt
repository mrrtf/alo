add_library(genmap SHARED
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
        segmentationImpl0.h
        zone.h
        segmentationFactory.h)

add_library(segmentation1 SHARED
        segmentationImplementation1.cxx)

add_library(segmentationcontours SHARED
        generateTestPoints.cxx
        generateTestPoints.h
        segmentationContours.cxx
        segmentationContours.h)

include(GenerateExportHeader)
generate_export_header(genmap)

target_include_directories(genmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(genmap PUBLIC mapping_interface contour Boost::boost)

target_include_directories(segmentation1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(segmentation1 PUBLIC genmap)
#set_target_properties(segmentation1 PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_include_directories(segmentationcontours PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(segmentationcontours PUBLIC segmentation1)

install(TARGETS genmap segmentation1 segmentationcontours LIBRARY DESTINATION lib)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

find_package(benchmark)
if (benchmark_FOUND)
    add_subdirectory(bench)
endif ()