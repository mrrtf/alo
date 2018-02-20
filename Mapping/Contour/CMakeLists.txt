add_library(contour INTERFACE)

# this trick is needed (only for header-only library)
# to get the files know to IDEs, e.g. CLion
add_custom_target(contour_ide SOURCES
        BBox.h
        Contour.h
        ContourCreator.h
        ContourCreator.inl
        Edge.h
        Helper.h
        Interval.h
        Polygon.h
        SegmentTree.h
        SVGWriter.h
        Vertex.h)

target_include_directories(contour INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)

install(TARGETS contour INCLUDES DESTINATION include)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

