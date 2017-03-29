include_directories(${ROOT_INCLUDE_DIR})
add_library(dhlt SHARED AliHLTMUONMapping.cxx)
target_link_libraries(dhlt Core)
install(TARGETS dhlt LIBRARY DESTINATION lib)
