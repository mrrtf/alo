set(MODULE r23legacy)
# Sources in alphabetical order
set(SRCS
        O2Muon.cxx
        O2Muon.h
        )

# Headers from sources
string(REPLACE ".cxx" ".h" HDRS "${SRCS}")

# Generate the dictionary
# It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${CMAKE_CURRENT_SOURCE_DIR}")

## Dependencies (Root and AliRoot ones, order does not matter)
set(LIBDEPS Core STEERBase ESD RAWDatabase RAWDatarec MUONraw MUONrec MUONgraphics)

# Generate the ROOT map
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Add a library to the project using the object
add_library(${MODULE} SHARED ${SRCS} G__${MODULE}.cxx)
target_link_libraries(${MODULE} PUBLIC AliRoot::MUONgraphics)
target_include_directories(${MODULE} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}
        $<INSTALL_INTERFACE:include>)

# System dependent: Modify the way the library is build
if (${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif (${CMAKE_SYSTEM} MATCHES Darwin)

add_executable(o2muon-filter-raw o2muon-filter-raw.cxx)
target_link_libraries(o2muon-filter-raw ${MODULE})

# Installation
install(TARGETS ${MODULE}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(TARGETS o2muon-filter-raw RUNTIME DESTINATION bin)

install(FILES ${HDRS} DESTINATION include)

