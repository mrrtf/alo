add_library(MIDConverterUtility SHARED ConvertDE.cxx)
target_include_directories(MIDConverterUtility INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(MIDConverterUtility PROPERTIES PUBLIC_HEADER "ConvertDE.h")
install(TARGETS MIDConverterUtility LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include/mid)