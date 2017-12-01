
add_library(contour STATIC
        bbox.h
        bbox.h
        contour.h
        contourCreator.cxx
        contourCreator.h
        edge.h
        helper.h
        interval.h
        polygon.h
        segmentTree.h
        vertex.h)

target_include_directories(contour PUBLIC .)

add_subdirectory(test)

