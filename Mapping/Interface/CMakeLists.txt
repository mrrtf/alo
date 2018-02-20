add_library(MCHMappingInterface INTERFACE)

target_include_directories(MCHMappingInterface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCDIR}>)

target_link_libraries(MCHMappingInterface INTERFACE Boost::boost)

install(TARGETS MCHMappingInterface INCLUDES DESTINATION ${CMAKE_INSTALL_INCDIR})

add_custom_target(MCHMappingInterface_ide SOURCES Segmentation.h SegmentationCInterface.h)