add_library(segcontour SHARED
        generateTestPoints.cxx
        generateTestPoints.h
        segmentationContours.cxx
        segmentationContours.h
        segcontour_export.h)

include(GenerateExportHeader)
generate_export_header(segcontour)

target_include_directories(segcontour PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(segcontour PUBLIC contour segmentation)
set_target_properties(segcontour PROPERTIES CXX_VISIBILITY_PRESET hidden)

install(TARGETS segcontour LIBRARY DESTINATION lib)
install(FILES
        generateTestPoints.h
        segmentationContours.h
        ${CMAKE_CURRENT_BINARY_DIR}/segcontour_export.h DESTINATION include)
