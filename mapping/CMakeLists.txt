add_subdirectory(interface)
add_subdirectory(genmap)
#add_subdirectory(genmap2)

if (BUILD_TESTING)
    add_subdirectory(test)
endif ()