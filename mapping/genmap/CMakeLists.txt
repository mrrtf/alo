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
        segmentationContours.cxx
        segmentationContours.h
        segmentationFactory.cxx
        segmentationImpl0.h
        zone.h
        segmentationFactory.h)

target_include_directories(genmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(genmap PUBLIC mapping_interface contour Boost::boost)

install(TARGETS genmap LIBRARY DESTINATION lib)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

find_package(benchmark)
if (benchmark_FOUND)
    add_subdirectory(bench)
endif ()