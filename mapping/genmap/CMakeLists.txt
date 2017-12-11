add_library(genmap STATIC
        genMotifType.cxx
        genPadSize.cxx
        motifPosition.h
        motifPositionContours.cxx
        motifPositionContours.h
        motifPositionTwoPadSizes.h
        motifType.h
        pad.h
        segmentationContours.cxx
        segmentationContours.h
        segmentationFactory.cxx
        segmentationImpl0.h
        zone.h
        )

target_include_directories(genmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(genmap PUBLIC mapping_interface contour Boost::boost)

install(TARGETS genmap ARCHIVE DESTINATION lib)

add_subdirectory(test)

find_package(benchmark)
if (benchmark_FOUND)
    add_subdirectory(bench)
endif ()