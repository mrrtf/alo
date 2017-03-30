set(MODULE dhlt)
add_definitions(-D_MODULE_="${MODULE}")

include_directories(${alo_SOURCE_DIR}/${MODULE})

include_directories(${ROOT_INCLUDE_DIR}
                    ${ALIROOT_INCLUDE_DIR}
                    )

set(SRCS
    AliHLTMUONMapping.cxx
    AliHLTMUONDigitReaderComponent.cxx
    AliHLTMUONDigitBlock.cxx
    )

string(REPLACE ".cxx" ".h" HDRS "${SRCS}")# only for those classes that do need a dictionary

# Generate the dictionary
# It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
get_directory_property(incdirs INCLUDE_DIRECTORIES)
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${incdirs}")

# Dependencies
set(ROOT_DEPENDENCIES Core Tree)
set(ALIROOT_DEPENDENCIES AliHLTMUON)

# Generate the ROOT map
# Dependecies
set(LIBDEPS ${ALIROOT_DEPENDENCIES} ${ROOT_DEPENDENCIES})
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Create an object to be reused in case of static libraries 
# Otherwise the sources will be compiled twice
add_library(${MODULE}-object OBJECT ${SRCS} G__${MODULE}.cxx)
# Add a library to the project using the object
add_library(${MODULE} SHARED $<TARGET_OBJECTS:dhlt-object>)

# linking
target_link_libraries(${MODULE} ${ALIROOT_DEPENDENCIES} ${ROOT_DEPENDENCIES})

# Setting the correct headers for the object as gathered from the dependencies
target_include_directories(${MODULE}-object PUBLIC $<TARGET_PROPERTY:${MODULE},INCLUDE_DIRECTORIES>)
set_target_properties(${MODULE}-object PROPERTIES COMPILE_DEFINITIONS $<TARGET_PROPERTY:${MODULE},COMPILE_DEFINITIONS>)

# Additional compilation flags
set_target_properties(${MODULE}-object PROPERTIES COMPILE_FLAGS " ")

# System dependent: Modify the way the library is build
if(${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(${CMAKE_SYSTEM} MATCHES Darwin)

# Installation
install(TARGETS ${MODULE}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES ${HDRS} DESTINATION include)

