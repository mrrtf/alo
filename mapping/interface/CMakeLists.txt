add_library(mapping_interface INTERFACE)

target_include_directories(mapping_interface INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCDIR}>)

target_link_libraries(mapping_interface INTERFACE Boost::boost)

install(TARGETS mapping_interface EXPORT mch-mapping INCLUDES DESTINATION ${CMAKE_INSTALL_INCDIR})
install(EXPORT mch-mapping DESTINATION cmake)

