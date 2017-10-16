
add_library(contour STATIC
        bbox.h
        contour.h
        contourCreator.h
        contourCreator.cxx
        edge.h
        interval.h
        polygon.h
        segmentTree.h
        vertex.h
        helper.h bbox.h)

add_subdirectory(test)

