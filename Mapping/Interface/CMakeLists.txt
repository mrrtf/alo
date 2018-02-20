add_library(MappingInterface INTERFACE)

target_include_directories(MappingInterface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCDIR}>)

target_link_libraries(MappingInterface INTERFACE Boost::boost)

install(TARGETS MappingInterface INCLUDES DESTINATION ${CMAKE_INSTALL_INCDIR})

add_custom_target(MappingInterface_ide SOURCES Segmentation.h SegmentationCInterface.h)