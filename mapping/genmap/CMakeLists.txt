add_library(genmap STATIC
        genDESegmentationFactory.cxx
        genDetectionElement.cxx
        genMotifType.cxx
        genMotifType.h
        genPadSize.cxx
        genSegmentationFactory.cxx
        segmentationImpl0.h
        motifPositionContours.h
        motifPosition.h
        motifPositionTwoPadSizes.h
        segmentationContours.cxx
        segmentationContours.h
        zone.h
        motifPositionContours.cxx)

target_include_directories(genmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(genmap PUBLIC mapping_interface contour Boost::boost)

install(TARGETS genmap ARCHIVE DESTINATION lib)

add_subdirectory(test)
