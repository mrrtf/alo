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
#include "codeWriter.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace rapidjson;

namespace {
std::string returnVectorAsString(const std::vector<int>& v)
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

void generateCode(const Value& motif, std::ostringstream& code)
{
  const Value& pads = motif["pads"];
  assert(pads.IsArray());

  unsigned int nofpads = pads.Size();

  std::vector<int> berg, ix, iy;

  for (const auto& p: pads.GetArray()) {
    assert(p["berg"].IsInt());
    assert(p["ix"].IsInt());
    assert(p["iy"].IsInt());
    berg.push_back(p["berg"].GetInt());
    ix.push_back(p["ix"].GetInt());
    iy.push_back(p["iy"].GetInt());
  }

  std::string motifID = motif["id"].GetString();

  code << "MotifType("
       << returnVectorAsString(berg)
       << ","
       << returnVectorAsString(ix)
       << ","
       << returnVectorAsString(iy)
       << ","
       << berg.size()
       << ")";
}
}


std::pair<std::string, std::string> generateCodeForMotifTypes(const rapidjson::Value& motifs)
{
  assert(motifs.IsArray());

  std::ostringstream decl, impl;

  decl << "#include <array>\n";
  decl << "#include <algorithm>\n";
  decl << "#include <iostream>\n";
  decl << "#include <vector>\n";

  decl << mappingNamespaceBegin();
  decl << "\n";
  decl << "class MotifType {\n";
  decl << "public:\n";
  decl << "  MotifType(const std::array<int,64>& berg, const std::array<int,64>& ix, const std::array<int,64>& iy, int nofPads) :\n";
  decl << "  mBerg(berg), mIx(ix), mIy(iy), mNofPads(nofPads) {\n";
  decl << "  }\n";
  decl << "  int NofPads() { return mNofPads; }\n";
  decl << "  int NofPadsX() { \n";
  decl << "    auto result = std::minmax_element(mIx.begin(),mIx.begin()+mNofPads);\n";
  decl << "    return 1+*result.second - *result.first;\n";
  decl << "  }\n";
  decl << "  int NofPadsY() { \n";
  decl << "    auto result = std::minmax_element(begin(mIy), end(mIy)+mNofPads);\n";
  decl << "    return 1+*result.second - *result.first;\n";
  decl << "  }\n";
  decl << "private:\n";
  decl << "  std::array<int,64> mBerg;\n";
  decl << "  std::array<int,64> mIx;\n";
  decl << "  std::array<int,64> mIy;\n";
  decl << "  int mNofPads;\n";
  decl << "};\n";
  decl << "\n";

  decl << "std::vector<MotifType> MotifTypes();\n";
  decl << mappingNamespaceEnd();

  int n{0};
  //impl << "#include \"" << filename << ".h\"\n";
  impl << mappingNamespaceBegin();
  impl << "std::vector<MotifType> MotifTypes() {\n";
  impl << "return std::vector<MotifType>{";
  for (const auto& motifType: motifs.GetArray()) {
    assert(motifType.IsObject());
    generateCode(motifType, impl);
    ++n;
    if (n < motifs.Size()) { impl << ",\n"; }
  }
  impl << "\n};\n";
  impl << "}\n";
  impl << mappingNamespaceEnd();

  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
};