add_subdirectory(interface)
add_subdirectory(impl1)
add_subdirectory(impl2)
add_subdirectory(impl3)
add_subdirectory(segcontour)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()