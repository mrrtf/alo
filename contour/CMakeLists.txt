
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

target_include_directories(contour PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(contour PROPERTIES CXX_VISIBILITY_PRESET hidden)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

