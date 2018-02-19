add_library(contour INTERFACE)

# this trick is needed (only for header-only library)
# to get the files know to IDEs, e.g. CLion
add_custom_target(contour_ide SOURCES
        bbox.h
        contour.h
        contourCreator.h
        contourCreator.inl
        edge.h
        helper.h
        interval.h
        polygon.h
        segmentTree.h
        svgWriter.h
        vertex.h)

target_include_directories(contour INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)

install(TARGETS contour INCLUDES DESTINATION include)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

