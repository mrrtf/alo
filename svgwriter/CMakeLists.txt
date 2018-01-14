add_library(svgwriterlib
        svgContour.cxx
        svgContour.h)

target_include_directories(svgwriterlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(svgwriterlib contour)

add_executable(svgwriter svgWriter.cxx)
target_link_libraries(svgwriter svgwriterlib segcontour)

