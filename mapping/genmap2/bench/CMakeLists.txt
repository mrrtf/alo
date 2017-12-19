add_executable(benchBGI benchBGI.cxx)
target_link_libraries(benchBGI PRIVATE benchmark::benchmark segmentationcontours)
