
add_library(contour STATIC
        contour.h
        contourCreator.h
        contourCreator.cxx
        edge.h
        interval.h
        polygon.h
        segmentTree.h
        vertex.h
        helper.h)

add_subdirectory(test)

