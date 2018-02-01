
add_library(contour INTERFACE)

set(HEADERS
        bbox.h contour.h contourCreator.h
        contourCreator.inl
        edge.h
        helper.h
        interval.h
        polygon.h
        segmentTree.h
        svgWriter.h
        vertex.h)

# this trick is needed (only for header-only library)
# to get the files know to IDEs, e.g. CLion
add_custom_target(contour_ide SOURCES ${HEADERS})

target_include_directories(contour INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCDIR}>)

install(TARGETS contour INCLUDES DESTINATION include)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

