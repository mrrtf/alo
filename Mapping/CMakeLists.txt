add_subdirectory(Interface)
#add_subdirectory(impl1)
#add_subdirectory(impl2)
add_subdirectory(Impl3)
add_subdirectory(SegContour)
add_subdirectory(Contour)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

