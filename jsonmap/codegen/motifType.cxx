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


#include "motifType.h"
#include "jsonReader.h"
#include "writer.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace rapidjson;
namespace jsonmap {
namespace codegen {

namespace {
std::string returnVectorAsString(const std::vector<int> &v)
{

  std::ostringstream s;

  s << "std::array<int,64>{";

  for (std::vector<int>::size_type i = 0; i < v.size(); ++i) {
    s << std::setw(2) << v[i];
    if (i < v.size() - 1) {
      s << ",";
    }
  }
  s << "}";
  return s.str();
}

}

void generateCode(const Value &motif, std::ostringstream &code)
{
  const Value &pads = motif["pads"];
  assert(pads.IsArray());

  unsigned int nofpads = pads.Size();

  std::vector<int> berg, ix, iy;

  for (const auto &p: pads.GetArray()) {
    assert(p["berg"].IsInt());
    assert(p["ix"].IsInt());
    assert(p["iy"].IsInt());
    berg.push_back(p["berg"].GetInt());
    ix.push_back(p["ix"].GetInt());
    iy.push_back(p["iy"].GetInt());
  }

  std::string motifID = motif["id"].GetString();

  code << "/* " << motifID << " */  MotifType(\n  "
       << returnVectorAsString(berg)
       << ",\n  "
       << returnVectorAsString(ix)
       << ",\n  "
       << returnVectorAsString(iy)
       << ",\n  "
       << berg.size()
       << ")";
}

std::string generateCodeForMotifTypes(const rapidjson::Value &motifs)
{
  assert(motifs.IsArray());

  std::ostringstream impl;

  int n{0};
  impl << generateInclude({"motifType.h"});
  impl << mappingNamespaceBegin("impl1");
  impl << "MotifTypeArray arrayOfMotifTypes{\n";
  for (const auto &motifType: motifs.GetArray()) {
    assert(motifType.IsObject());
    impl << "/* index: " << n << " */ ";
    generateCode(motifType, impl);
    ++n;
    if (n < motifs.Size()) { impl << ",\n"; }
  }
  impl << "\n};\n";
  impl << mappingNamespaceEnd("impl1");

  return impl.str();
}

}
}