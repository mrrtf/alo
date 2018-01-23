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


#include <sstream>
#include <vector>
#include "segmentationCommon.h"

using namespace rapidjson;

namespace jsonmap {
namespace codegen {

std::string generateCodeForDetElemIdArray(const rapidjson::Value &detection_elements)
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

  impl << "\n  const std::array<int," << deids.size() << "> detElemIndexToDetElemId{ \n";
  for (auto i = 0; i < deids.size(); ++i) {
    impl << deids[i];
    if (i < deids.size() - 1) { impl << ","; }
  }
  impl << "};\n";
  return impl.str();
}

std::string generateCodeForSegTypeArray(const Value &segmentations, const Value &detection_elements)
{
  std::ostringstream impl;

  impl << "\n  const std::array<int," << detection_elements.Size() << "> detElemIndexToSegType{";

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

  impl << "};\n";

  return impl.str();
}

}}

