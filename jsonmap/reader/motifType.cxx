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
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace rapidjson;

namespace {
std::string returnVectorAsString(const std::vector<int>& v) {

  std::ostringstream s;

  s << "{ return {";

  for ( std::vector<int>::size_type i = 0; i < v.size(); ++i ) {
    s << std::setw(2) << v[i];
    if (i < v.size()-1) {
      s << ",";
    }
  }
  s << "}; }";
  return s.str();
}

void generateCode(const Value& motif, std::ostringstream& decl, std::ostringstream& impl)
{
  const Value& pads = motif["pads"];
  assert(pads.IsArray());

  unsigned int nofpads = pads.Size();

  std::vector<int> berg,ix,iy;

  for (const auto& p: pads.GetArray()) {
    assert(p["berg"].IsInt());
    assert(p["ix"].IsInt());
    assert(p["iy"].IsInt());
    berg.push_back(p["berg"].GetInt());
    ix.push_back(p["ix"].GetInt());
    iy.push_back(p["iy"].GetInt());
  }

  std::string motifID = motif["id"].GetString();

  decl << "struct MotifTypeData" << motifID << " {\n"
    << "  static constexpr std::array<int," << nofpads << "> Berg() " << returnVectorAsString(berg) << ";\n"
    << "  static constexpr std::array<int," << nofpads << ">   Ix() " << returnVectorAsString(ix) << ";\n"
    << "  static constexpr std::array<int," << nofpads << ">   Iy() " << returnVectorAsString(iy) << ";\n"
   << "};\n";

  decl << "\n";
  decl << "typedef MotifType<MotifTypeData" << motifID << "> MotifType" << motifID << ";\n";
  decl << "\n";
}
}

std::pair<std::string, std::string> readMotifTypes(const std::string& inputfile)
{

  InputWrapper docw(inputfile.c_str());
  Document& doc = docw.document();
  if (!doc.HasMember("motifs")) {
    std::cout << "inputfile " << inputfile << " does not contain motif information" << std::endl;
  }
  const Value& motifs = doc["motifs"];
  assert(motifs.IsArray());

  std::ostringstream decl, impl;

  decl << "#include <array>\n";
  decl << "#include <algorithm>\n";
  decl << "#include <iostream>\n";

  decl << "\n";
  decl << "template<typename T> class MotifType {\n";
  decl << "public:\n";
  decl << "  static constexpr int NofPads() { return T::Berg().size(); }\n";
  decl << "  static constexpr int NofPadsX() { \n";
  decl << "    auto v = T::Ix();\n";
  decl << "    auto result = std::minmax_element(begin(v), end(v));\n";
  decl << "    return 1+*result.second - *result.first;\n";
  decl << "  }\n";
  decl << "  static constexpr int NofPadsY() { \n";
  decl << "    auto v = T::Iy();\n";
  decl << "    auto result = std::minmax_element(begin(v), end(v));\n";
  decl << "    return 1+*result.second - *result.first;\n";
  decl << "  }\n";
  decl << "};\n";
  decl << "\n";
  decl << "void testMotifTypes();\n";

  for (const auto& motifType: motifs.GetArray()) {
    assert(motifType.IsObject());
    generateCode(motifType, decl, impl);
  }

  impl << "#include \"motif.h\"\n";

  impl << "void testMotifTypes() {\n";

  for (const auto& motifType: motifs.GetArray()) {
    assert(motifType.IsObject());
    std::string id("MotifType");

    id += motifType["id"].GetString();

  impl << "std::cout << \"" << motifType["id"].GetString() << "\" << ' ' << " << id << "::NofPads() << ' ' << " << id << "::NofPadsX() << ' ' << " << id << "::NofPadsY() << std::endl;\n";
  }


  impl << "}\n";

  return std::make_pair<std::string, std::string>(decl.str(), impl.str());
};