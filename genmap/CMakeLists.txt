add_library(genmap STATIC genMotifType.h genMotifType.cxx genDetectionElement.h genDetectionElement.cxx genPadSize.cxx genSegmentation.cxx genSegmentation.h MotifPosition.cxx MotifPosition.h padfinder.h padfinder.cxx)

add_subdirectory(test)