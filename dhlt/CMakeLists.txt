set(MODULE dhlt)
add_definitions(-D_MODULE_="${MODULE}")

include_directories(${alo_SOURCE_DIR}/${MODULE})

include_directories(${ROOT_INCLUDE_DIR}
                    ${ALIROOT_INCLUDE_DIR}
                    )

set(SRCS
    Agent.cxx
    AliHLTMUONClusterWriterComponent.cxx
    AliHLTMUONDigitBlock.cxx
    AliHLTMUONDigitLoaderComponent.cxx
    AliHLTMUONDigitReaderComponent.cxx
    AliHLTMUONMapping.cxx
    AliHLTMUONNewClusterFinderComponent.cxx
    AliHLTMUONPreclusterBlock.cxx
    AliHLTMUONPreclusterFinderComponent.cxx
    )

string(REPLACE ".cxx" ".h" HDRS "${SRCS}")# only for those classes that do need a dictionary

# Generate the dictionary
# It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
get_directory_property(incdirs INCLUDE_DIRECTORIES)
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${incdirs}")

# Dependencies (Root and AliRoot ones, order does not matter)
set(LIBDEPS STEERBase STEER CDB ESD RAWDatabase RAWDatarec RAWDatasim HLTbase MUONraw MUONcore MUONcalib MUONmapping
    Core MUONsim MUONevaluation MUONtrigger MUONgeometry MUONrec Tree)

# Generate the ROOT map
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Add a library to the project using the object
add_library(${MODULE} SHARED ${SRCS} G__${MODULE}.cxx)
target_link_libraries(${MODULE} ${LIBDEPS})

# System dependent: Modify the way the library is build
if(${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(${CMAKE_SYSTEM} MATCHES Darwin)

# Installation
install(TARGETS ${MODULE}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES ${HDRS} DESTINATION include)

