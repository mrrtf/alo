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
#include "detectionElement.h"

#include "jsonReader.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "codeWriter.h"
#include <algorithm>

using namespace rapidjson;

std::string generateCodeForDetectionElements(const rapidjson::Value& des)
{
  assert(des.IsArray());
  std::vector<int> deids;
  for (const auto& de: des.GetArray()) {
    assert(de.IsObject());
    assert(de["id"].IsInt());
    deids.push_back(de["id"].GetInt());
  }

  std::ostringstream impl;

  std::sort(deids.begin(),deids.end());

  impl << "#include <array>\n";
  impl << "#include <algorithm>\n";
  impl << mappingNamespaceBegin() << "\n";
  impl << "std::array<int," << deids.size() << "> deids = { ";
  for ( auto i = 0; i < deids.size(); ++i ) {
    impl << deids[i];
    if  ( i < deids.size() - 1 ) impl << ",";
  }
  impl << "};\n";
  impl << R"(
int getDetElemIdFromDetElemIndex(int deIndex) {
  if (deIndex < 0 || deIndex>=deids.size()) throw std::out_of_range("deIndex out of range");
  return deids[deIndex];
}

int getDetElemIndexFromDetElemId(int deId) {
  return std::distance(deids.begin(),std::find(deids.begin(),deids.end(),deId));
}
)";
  impl << mappingNamespaceEnd();

  return impl.str();
}

