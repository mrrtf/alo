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


#include "padGroupType.h"
#include <vector>
#include <sstream>
#include "writer.h"
#include <string>
#include <iomanip>
#include <map>

using rapidjson::Value;
namespace jsonmap {
namespace codegen {

namespace {
std::string returnVectorAsString(const std::vector<int> &v)
{

  std::ostringstream s;

  s << "    {";

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

struct triplet
{

    std::vector<int> berg;
    std::vector<int> ix;
    std::vector<int> iy;

};

void output(int i, bool isSplit, const std::string &basemotif, std::ostream &out, const triplet &v)
{
  out << "  if (i==" << i << ") { /* " << basemotif << (isSplit ? " split " : " ") << v.berg.size()
      << " pads */\n    return PadGroupType(\n  "
      << returnVectorAsString(v.berg)
      << ",\n  "
      << returnVectorAsString(v.ix)
      << ",\n  "
      << returnVectorAsString(v.iy)
      << ");\n  }\n";
}

int splitAtX(const triplet &input, triplet &out1, triplet &out2, int x)
{
  // if ix = x then fill out2 else fill out1
  out1 = {};
  out2 = {};
  for (auto i = 0; i < input.ix.size(); ++i) {
    triplet *t = &out1;
    if (input.ix[i] >= x) {
      t = &out2;
    }
    t->ix.push_back(input.ix[i]);
    t->iy.push_back(input.iy[i]);
    t->berg.push_back(input.berg[i]);
  }
  return 2;
}

int splitAtY(const triplet &input, triplet &out1, triplet &out2, int y)
{
  // if iy = y then fill out2 else fill out1
  out1 = {};
  out2 = {};
  for (auto i = 0; i < input.ix.size(); ++i) {
    triplet *t = &out1;
    if (input.iy[i] >= y) {
      t = &out2;
    }
    t->ix.push_back(input.ix[i]);
    t->iy.push_back(input.iy[i]);
    t->berg.push_back(input.berg[i]);
  }
  return 2;
}

int split(const std::string &motifID,
          const triplet &input,
          triplet &out1,
          triplet &out2)
{
  // For motiftypes which are used in motifs with two padsizes
  // (1BG,1NH,1NG) or that have non adjacent pads (E14,E15)
  // we split the motif in two groups to get
  // a motif with one pad size and adjacent pads.

  // don't fret about being generic, we have 5 special cases over 210, let's deal with them by hand
  out1 = input;
  out2 = {};
  if (motifID == "1BG") {
    return splitAtX(input, out1, out2, 3);
  } else if (motifID == "1NH") {
    return splitAtY(input, out1, out2, 3);
  } else if (motifID == "1NG") {
    return splitAtY(input, out1, out2, 3);
  } else if (motifID == "E14") {
    return splitAtY(input, out1, out2, 8);
  } else if (motifID == "E15") {
    return splitAtY(input, out1, out2, 4);
  }
  return 1;
}

int generateCode(int index, int nonregIndex, const rapidjson::Value &motifType, std::ostream &out)
{
  const Value &pads = motifType["pads"];
  assert(pads.IsArray());

  unsigned int nofpads = pads.Size();

  triplet v;

  for (const auto &p: pads.GetArray()) {
    assert(p["berg"].IsInt());
    assert(p["ix"].IsInt());
    assert(p["iy"].IsInt());
    v.berg.push_back(p["berg"].GetInt());
    v.ix.push_back(p["ix"].GetInt());
    v.iy.push_back(p["iy"].GetInt());
  }

  std::string motifID = motifType["id"].GetString();

  triplet v1, v2;

  int n = split(motifID, v, v1, v2);

  if (n == 2) {
    output(index, true, motifID, out, v1);
    output(nonregIndex, true, motifID, out, v2);
  } else {
    output(index, false, motifID, out, v1);
  }

  return n;
}

std::string generateCodeForPadGroupTypes(const rapidjson::Value &motiftypes)
{
  assert(motiftypes.IsArray());

  std::ostringstream impl;

  int n{0};
  impl << generateInclude({"padGroupType.h"});
  impl << mappingNamespaceBegin("impl2");
  impl << "PadGroupType getPadGroupType(int i) {\n";
  int nmotifs{static_cast<int>(motiftypes.GetArray().Size())};
  int nonreg{nmotifs};
  std::map<int,int> m; // for non regular motifs gives the relation ship motifTypeId -> padGroupTypeId
  for (auto i = 0; i < nmotifs; ++i) {
    const auto &motifType = motiftypes.GetArray()[i];
    assert(motifType.IsObject());
    int n = generateCode(i,nonreg, motifType, impl);
    if (n>1) {
      m[i]= nonreg++;
    }
  }
  impl << "  throw std::out_of_range(\"i should be between 0 and "
       << n << "\");\n";
  impl << "}\n";

  impl << mappingNamespaceEnd("impl2");

  return impl.str();
}

}
}
