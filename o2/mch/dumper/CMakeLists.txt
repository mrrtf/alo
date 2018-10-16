# FIXME: should call flatc to re-generate the header files
# from the .fbs files instead of using the ones commited ?

add_executable(mch-dump mch-dump.cxx SegmentationPair.cxx DumpDigits.cxx DumpClusters.cxx)
target_link_libraries(mch-dump PRIVATE o2::MCHMappingImpl3 Boost::program_options MCHDataFormat)
install(TARGETS mch-dump RUNTIME DESTINATION bin)

