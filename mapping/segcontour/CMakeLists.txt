set(SRCS
        generateTestPoints.cxx
        generateTestPoints.h
        segmentationContours.cxx
        segmentationContours.h)

foreach (impl RANGE 1 2 1)
    add_library(segcontour${impl} SHARED ${SRCS})
    target_include_directories(segcontour${impl} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(segcontour${impl} PUBLIC mapping_interface contour impl${impl})
    install(TARGETS segcontour${impl} LIBRARY DESTINATION lib)
endforeach ()

install(FILES
        generateTestPoints.h
        segmentationContours.h
        DESTINATION include)

