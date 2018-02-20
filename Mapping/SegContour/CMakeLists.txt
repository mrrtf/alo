find_package(Boost COMPONENTS program_options REQUIRED)

set(SRCS
        SegmentationContours.cxx
        SegmentationContours.h
        SegmentationSVGWriter.h
        SegmentationSVGWriter.cxx)

foreach (impl RANGE 3 3 1)

    add_library(SegContour${impl} SHARED ${SRCS})
    target_include_directories(SegContour${impl} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(SegContour${impl} PUBLIC contour MappingImpl${impl})

    add_executable(svgSegmentation${impl} SVGSegmentation.cxx)
    target_link_libraries(svgSegmentation${impl} PUBLIC SegContour${impl} Boost::program_options)
    install(TARGETS SegContour${impl} svgSegmentation${impl} LIBRARY DESTINATION lib RUNTIME DESTINATION bin)

endforeach ()

install(FILES
        SegmentationContours.h
        SegmentationSVGWriter.h
        DESTINATION include)

