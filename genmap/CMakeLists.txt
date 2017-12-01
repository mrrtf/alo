add_library(genmap SHARED genMotifType.h genMotifType.cxx genDetectionElement.h genDetectionElement.cxx genPadSize.cxx
        genSegmentationInterface.h
        genSegmentationFactory.h
        genSegmentationFactory.cxx
        genSegmentationImpl0.h
        genDESegmentationFactory.cxx
        genDESegmentationFactory.h
        motifPosition.h
        motifPositionTwoPadSizes.h
        pad.h)

target_include_directories(genmap PUBLIC .)
target_link_libraries(genmap PUBLIC contour Boost::boost)

install(TARGETS genmap LIBRARY DESTINATION lib)
install(FILES genSegmentationInterface.h genSegmentationFactory.h DESTINATION include)

add_subdirectory(test)
