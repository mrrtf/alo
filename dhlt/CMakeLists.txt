set(MODULE dhlt)
add_definitions(-D_MODULE_="${MODULE}")

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

generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}"
        "$<BUILD_INTERFACE:${ALIROOT}/include>;$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>;$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

# Dependencies (Root and AliRoot ones, order does not matter)
set(LIBDEPS STEERBase STEER CDB ESD RAWDatabase RAWDatarec RAWDatasim HLTbase MUONraw MUONcore MUONcalib MUONmapping
        Core MUONsim MUONevaluation MUONtrigger MUONgeometry MUONrec Tree)

# Generate the ROOT map
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Add a library to the project using the object
add_library(${MODULE} SHARED ${SRCS} G__${MODULE}.cxx)
target_link_libraries(${MODULE} PUBLIC AliRoot::HLTbase AliRoot::MUONrec)
target_include_directories(${MODULE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# System dependent: Modify the way the library is build
if (${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif (${CMAKE_SYSTEM} MATCHES Darwin)

# Installation
install(TARGETS ${MODULE}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES ${HDRS} DESTINATION include)

