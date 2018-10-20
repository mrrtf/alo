if(FlatBuffers_FOUND)
  add_subdirectory(esdconverter)
endif()

add_subdirectory(legacy)
add_subdirectory(dataformatconverter)
add_subdirectory(rawconverter)
add_subdirectory(trackrefsconverter)
