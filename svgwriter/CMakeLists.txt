foreach (impl RANGE 1 2 1)
    add_executable(svgwriter${impl} svgWriter.cxx)
    target_link_libraries(svgwriter${impl} segcontour${impl} contour)
endforeach ()
