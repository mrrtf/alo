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
#include "motifPosition.h"
#include "segmentationCommon.h"
#include "writer.h"
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using rapidjson::Value;

namespace jsonmap {
namespace codegen {
namespace impl1 {

std::string generateCodeForSegmentationFactory(const Value &segmentations, const Value &detection_elements)
{
  std::ostringstream impl;

  impl << generateInclude({"array", "iterator", "stdexcept", "vector"});

  impl << mappingNamespaceBegin("impl1");

  impl << generateCodeForDetElemIdArray(detection_elements);

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
  outputCode("", generateCodeForSegmentationFactory(segmentations, detection_elements), "SegmentationFactory",
             includeGuards, !standalone);
}

}}}

