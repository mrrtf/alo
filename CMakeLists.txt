cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

### CMP0025   Compiler id for Apple Clang is now AppleClang.
### CMP0042   MACOSX_RPATH is enabled by default.

FOREACH (p
    CMP0025 # CMake 3.0
    CMP0042 # CMake 3.0
    )
  IF (POLICY ${p})
    cmake_policy(SET ${p} NEW)
  ENDIF ()
endforeach ()

project(alo)

set(CMAKE_MODULE_PATH "${alo_SOURCE_DIR}/cmake")

find_package(RapidJSON)

if(ROOTSYS)
find_package(ROOT 6.08 REQUIRED)
else(ROOTSYS)
    message(FATAL_ERROR "root required")
endif(ROOTSYS)

if(ALIROOT)
find_package(ALIROOT)
else(ALIROOT)
    message(FATAL_ERROR "aliroot required")
endif(ALIROOT)

include(CMakeALICE)

if(RAPIDJSON_FOUND)
  add_subdirectory(jsonmap)
endif()
add_subdirectory(dhlt)
add_subdirectory(binmap)
