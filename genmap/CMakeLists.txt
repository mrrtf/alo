add_library(genmap STATIC genMotifType.h genMotifType.cxx genDetectionElement.h genDetectionElement.cxx genPadSize.cxx Segmentation.cxx Segmentation.h genSegmentation.cxx genSegmentation.h HasIntegerIdWithinGivenRange.h)

add_subdirectory(test)