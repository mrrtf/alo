
add_executable(mch-mapping-generate-code mch-mapping-generate-code.cxx
        jsonReader.h
        writer.h writer.cxx
        chamber.h chamber.cxx
        motifType.h motifType.cxx
        padSize.h padSize.cxx
        segmentationCommon.h segmentationCommon.cxx
        segmentation1.h segmentation1.cxx
        segmentation2.h segmentation2.cxx
        motifPosition.h motifPosition.cxx
        motifPosition1.h motifPosition1.cxx
        motifPosition2.h motifPosition2.cxx
        padGroupType.h padGroupType.cxx
        padGroup.h padGroup.cxx
        report.cxx report.h)

target_link_libraries(mch-mapping-generate-code Boost::program_options RapidJSON)

install(TARGETS mch-mapping-generate-code RUNTIME DESTINATION bin)

