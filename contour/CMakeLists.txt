
add_library(contour STATIC
        cantypefitvalue.h
        contour.cxx
        contour.h
        edge.h
        interval.h
        polygon.cxx
        polygon.h
        segment.h
        segmentTree.h
        vertex.h
        helper.h)

add_subdirectory(test)

