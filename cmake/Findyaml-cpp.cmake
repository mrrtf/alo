find_package(yaml-cpp CONFIG)

if(TARGET yaml-cpp)
  get_target_property(incdirs yaml-cpp INTERFACE_INCLUDE_DIRECTORIES)
  if(NOT incdirs)
    set_target_properties(yaml-cpp
                          PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                     ${YAML_CPP_INCLUDE_DIR})
  endif()
else()
  message(FATAL could not find yaml-cpp)
endif()
