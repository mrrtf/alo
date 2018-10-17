add_executable(dhlt-generate-binmapfile dhlt-generate-binmapfile.cxx)
target_link_libraries(dhlt-generate-binmapfile dhlt)
install(TARGETS dhlt-generate-binmapfile RUNTIME DESTINATION bin)

