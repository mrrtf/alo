//
// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See https://alice-o2.web.cern.ch/ for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// @author  Laurent Aphecetche


#include "segmentation1.h"
#include "writer.h"
#include "motifPosition.h"
#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>

using rapidjson::Value;

namespace jsonmap {
namespace codegen {
namespace impl1 {
std::string generateCodeForOneSegmentation(int index, bool isBending,
                                           const Value &segmentations,
                                           const Value &motiftypes,
                                           const Value &padsizes)
{
  std::vector<MotifPosition> motifpositions = getMotifPositions(index, isBending, segmentations, motiftypes, padsizes);

  std::ostringstream code;

  std::string bendingString = (isBending ? "true" : "false");
  code << "template<>\n";
  code << "Segmentation<" << index << "," << bendingString
       << ">::Segmentation(const MotifTypeArray& motifTypes)"
       << " : mId(" << index << "),mIsBendingPlane(" << bendingString << "), "
       << "mNofPads{0},"
       << "mMotifPositions{o2::mch::mapping::getMotifPositions<" << index << "," << bendingString << ">()}\n";

  code << "{";

  code << "  populatePads(motifTypes);\n";
  code << "  createContours(motifTypes);\n";

  code << "}\n";

  return code.str();
}

std::string
generateCodeForSegmentationType(int index, const Value &segmentations, const Value &motiftypes,
                                const Value &padsizes)
{
  std::string code;
  for (auto bending: {true, false}) {
    code += generateCodeForOneSegmentation(index, bending, segmentations, motiftypes, padsizes);
  }
  return code;
}

std::string generateCodeForSegTypeArray(const Value &segmentations, const Value &detection_elements)
{
  std::ostringstream impl;

  impl << "namespace {";

  impl << "\n  std::array<int," << detection_elements.Size() << "> segtype{";

  for (int ide = 0; ide < detection_elements.GetArray().Size(); ++ide) {
    const auto &de = detection_elements.GetArray()[ide];
    for (int i = 0; i < segmentations.Size(); ++i) {
      const auto &seg = segmentations.GetArray()[i];
      if (!strcmp(seg["segtype"].GetString(), de["segtype"].GetString())) {
        impl << i;
        if (ide < detection_elements.GetArray().Size() - 1) { impl << ","; }
        break;
      }
    }
  }

  impl << "};\n}";

  return impl.str();
}

std::string generateCodeForDetectionElements(const rapidjson::Value &detection_elements)
{
  assert(detection_elements.IsArray());
  std::vector<int> deids;
  for (const auto &de: detection_elements.GetArray()) {
    assert(de.IsObject());
    assert(de["id"].IsInt());
    deids.push_back(de["id"].GetInt());
  }

  std::ostringstream impl;

  std::sort(deids.begin(), deids.end());

  impl << "std::vector<int> getDetElemIds() { \n";
  impl << "return {\n";
  for (auto i = 0; i < deids.size(); ++i) {
    impl << deids[i];
    if (i < deids.size() - 1) { impl << ","; }
  }
  impl << "};\n}\n";
  return impl.str();
}

std::string generateCodeForSegmentationFactory(const Value &segmentations, const Value &detection_elements)
{
  std::ostringstream impl;

  impl << generateInclude({"array", "iterator", "stdexcept", "vector"});

  impl << mappingNamespaceBegin("impl1");

  impl << generateCodeForDetectionElements(detection_elements);

  impl << generateCodeForSegTypeArray(segmentations, detection_elements);

  impl << R"(
  std::unique_ptr<SegmentationInterface> getSegmentationByType(int type, bool isBendingPlane) {
)";

  for (int i = 0; i < 21; ++i) {
    for (auto b : std::array<bool, 2>{true, false}) {
      impl << "    if (isBendingPlane==" << (b ? "true" : "false") << " && type==" << i << ") {\n";
      impl << "      return std::unique_ptr<SegmentationInterface>{new Segmentation<" << i << ","
           << (b ? "true" : "false") << ">{arrayOfMotifTypes}};\n";
      impl << "    };\n";
    }
  }

  impl
    << "  throw std::out_of_range(std::to_string(type) + \" is not a valid segtype\");\n";
  impl << "}\n";
  impl << mappingNamespaceEnd("impl1");

  return impl.str();
}

void generateCodeForSegmentations(const Value &segmentations, const Value &motiftypes,
                                  const Value &padsizes,
                                  const Value &detection_elements)
{
  bool includeGuards{true};
  bool standalone{true};
  outputCode("", generateCodeForSegmentationFactory(segmentations, detection_elements), "genSegmentationFactory",
             includeGuards, !standalone);
}

}}}

