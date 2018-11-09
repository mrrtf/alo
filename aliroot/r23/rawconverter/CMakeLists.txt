add_executable(mid-convert-raw
        mid-convert-raw.cxx 
        RawToMID.cxx
        )

target_link_libraries(mid-convert-raw PUBLIC Boost::program_options Boost::serialization AliRoot::MUONsim AliRoot::MUONbase MIDDataFormat MIDUtility MIDConverterUtility)
install(TARGETS mid-convert-raw RUNTIME DESTINATION bin)
