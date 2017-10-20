add_library(genmap STATIC genMotifType.h genMotifType.cxx genDetectionElement.h genDetectionElement.cxx genPadSize.cxx
    genSegmentationInterface.h
    genSegmentationFactory.h
    genSegmentationFactory.cxx
    genSegmentationImpl0.h
    )

target_include_directories(genmap SYSTEM PUBLIC ${Boost_INCLUDE_DIR} ../contour)
target_link_libraries(genmap contour)

add_subdirectory(test)
