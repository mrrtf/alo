find_package(Boost COMPONENTS program_options REQUIRED)

set(SRCS
        SegmentationContours.cxx
        SegmentationContours.h
        SegmentationSVGWriter.h
        SegmentationSVGWriter.cxx)

foreach (impl RANGE 3 3 1)

    add_library(MCHMappingSegContour${impl} SHARED ${SRCS})
    target_include_directories(MCHMappingSegContour${impl} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(MCHMappingSegContour${impl} PUBLIC MCHMappingContour MCHMappingImpl${impl})

    add_executable(mch-mapping-svg-segmentation${impl} SVGSegmentation.cxx)
    target_link_libraries(mch-mapping-svg-segmentation${impl} PUBLIC MCHMappingSegContour${impl} Boost::program_options)
    install(TARGETS MCHMappingSegContour${impl} mch-mapping-svg-segmentation${impl} LIBRARY DESTINATION lib RUNTIME DESTINATION bin)

endforeach ()

install(FILES SegmentationContours.h
        SegmentationSVGWriter.h
        DESTINATION include)

