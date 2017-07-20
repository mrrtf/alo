add_library(genmap STATIC genMotifType.h genMotifType.cxx genDetectionElement.h genDetectionElement.cxx genPadSize.cxx
        genSegmentationInterface.h
        genSegmentationFactory.h
        genSegmentationFactory.cxx
        genSegmentationImpl0.h
        )

add_subdirectory(test)