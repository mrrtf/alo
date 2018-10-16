# FIXME: should call flatc to re-generate the header files
# from the .fbs files instead of using the ones commited ?

add_library(MCHDataFormat INTERFACE)
target_include_directories(MCHDataFormat INTERFACE .)
target_link_libraries(MCHDataFormat INTERFACE flatbuffers::flatbuffers)
install(DIRECTORY . DESTINATION include)
