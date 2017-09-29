add_library(genmap STATIC genMotifType.h genMotifType.cxx genDetectionElement.h genDetectionElement.cxx genPadSize.cxx
    genSegmentationInterface.h
    genSegmentationFactory.h
    genSegmentationFactory.cxx
    genSegmentationImpl0.h
    )

target_include_directories(genmap SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

add_subdirectory(test)

