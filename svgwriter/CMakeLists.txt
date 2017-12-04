add_library(svgwriterlib
        svgContour.cxx
        svgContour.h
        svgSegmentationInterface.h
        svgSegmentationInterface.cxx)

target_include_directories(svgwriterlib PUBLIC .)
target_link_libraries(svgwriterlib genmap contour)

add_executable(svgwriter svgWriter.cxx)
target_link_libraries(svgwriter svgwriterlib)

