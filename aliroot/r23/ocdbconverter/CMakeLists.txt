add_executable(mid-convert-ocdb
        mid-convert-ocdb.cxx 
        HvToMID.cxx
        )

target_link_libraries(mid-convert-ocdb PUBLIC Boost::program_options Boost::serialization AliRoot::MUONsim AliRoot::MUONbase ROOT::Core)
install(TARGETS mid-convert-ocdb RUNTIME DESTINATION bin)
