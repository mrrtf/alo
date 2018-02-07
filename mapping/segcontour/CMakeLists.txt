find_package(Boost COMPONENTS program_options REQUIRED)

set(SRCS
        segmentationContours.cxx
        segmentationContours.h
        segmentationSVGWriter.h
        segmentationSVGWriter.cxx)

foreach (impl RANGE 2 3 1)

    add_library(segcontour${impl} SHARED ${SRCS})
    target_include_directories(segcontour${impl} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(segcontour${impl} PUBLIC mapping_interface contour impl${impl})

    add_executable(svgSegmentation${impl} svgSegmentation.cxx)
    target_link_libraries(svgSegmentation${impl} PUBLIC segcontour${impl} Boost::program_options)
    install(TARGETS segcontour${impl} svgSegmentation${impl} LIBRARY DESTINATION lib RUNTIME DESTINATION bin)

endforeach ()

install(FILES
        segmentationContours.h
        segmentationSVGWriter.h
        DESTINATION include)

