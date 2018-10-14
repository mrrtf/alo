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
#include "writer.h"
#include <algorithm>
#include <iomanip>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include "boost/format.hpp"

using rapidjson::Value;
namespace jsonmap {
namespace codegen {
namespace impl2 {

int extent(const std::vector<int> &v)
{
  auto result = std::minmax_element(begin(v), end(v));
  return 1 + *result.second - *result.first;
}

std::string returnVectorAsString(const std::vector<int> &v)
{
  std::ostringstream s;
  s << "{";
  for (std::vector<int>::size_type i = 0; i < v.size(); ++i) {
    s << boost::format("%2d") % v[i];
    if (i < v.size() - 1) {
      s << ",";
    }
  }
  s << "}";
  return s.str();
}

int PadGroupType::nofPadsX() const
{
  return extent(ix);
}

int PadGroupType::nofPadsY() const
{
  return extent(iy);
}

int PadGroupType::getIndex(int ix, int iy) const
{
  return ix + iy * nofPadsX();
}

std::vector<int> PadGroupType::fastIndex() const
{
  std::vector<int> fi;

  fi.resize(nofPadsX() * nofPadsY(), -1);

  for (auto i = 0; i < ix.size(); ++i) {
    fi[getIndex(ix[i], iy[i])] = channelId[i];
  }
  return fi;
}

std::ostream &operator<<(std::ostream &out, const PadGroupType &v)
{
  out << "  if (i==" << v.id << ") { /* " << v.originalMotifTypeIdString
      << " (" << v.originalMotifTypeId << ") "
      << (v.isSplit ? " split " : " ") << v.channelId.size()
      << " pads */\n    return PadGroupType("
      << boost::format("%2d,%2d,") % v.nofPadsX() % v.nofPadsY()
      << returnVectorAsString(v.fastIndex())
      << ");\n  }";

  return out;
}

void shift(std::vector<int> &vect)
{
  // shift all values by the minimum so values starts at zero
  int m = *(std::min_element(vect.begin(), vect.end()));
  for (auto &v: vect) {
    v -= m;
  }
}

PadGroupType extract(int index, const PadGroupType &orig, const std::vector<int> &indices)
{
  PadGroupType out{orig};
  out.id = index;
  out.isSplit = true;
  out.clear();
  for (auto i: indices) {
    out.channelId.push_back(orig.channelId[i]);
    out.ix.push_back(orig.ix[i]);
    out.iy.push_back(orig.iy[i]);
  }
  shift(out.ix);
  shift(out.iy);
  return out;
}

int splitBetween(int regIndex, int nonRegIndex, const PadGroupType &input, PadGroupType &out1,
                 PadGroupType &out2, int valueLow, int valueHigh, bool isX)
{
  // if ix = x then fill out2 else fill out1

  std::vector<int> i1, i2;

  for (auto i = 0; i < input.ix.size(); ++i) {
    bool mustSplit{false};

    if (isX && input.ix[i] >= valueLow && input.ix[i] <= valueHigh) {
      mustSplit = true;
    } else if (!isX && input.iy[i] >= valueLow && input.iy[i] <= valueHigh) {
      mustSplit = true;
    }

    if (mustSplit) {
      i1.push_back(i);
    } else {
      i2.push_back(i);
    }
  }

  if (i2.size() > i1.size()) {
    std::swap(i1, i2);
  }
  out1 = extract(regIndex, input, i1);
  out2 = extract(nonRegIndex, input, i2);

  return 2;
}

int split(int regIndex, int nonRegIndex, const std::string &motifID,
          const PadGroupType &input,
          PadGroupType &out1,
          PadGroupType &out2,
          PadGroupType &out3)
{
  // For motiftypes which are used in motifs with two padsizes
  // (1BG,1NH,1NG) or that have non adjacent pads (E14,E15)
  // we split the motif in two groups to get
  // a motif with one pad size and adjacent pads.

  // don't fret about being generic, we have 5 special cases over 210, let's deal with them by hand
  if (motifID == "1BG") {
    return splitBetween(regIndex, nonRegIndex, input, out1, out2, 3, 64, true);
  } else if (motifID == "1NH") {
    return splitBetween(regIndex, nonRegIndex, input, out1, out2, 3, 64, false);
  } else if (motifID == "1NG") {
    return splitBetween(regIndex, nonRegIndex, input, out1, out2, 3, 64, false);
  } else if (motifID == "E14") {
    PadGroupType dummy;
    splitBetween(regIndex, nonRegIndex, input, out1, dummy, 8, 64, false);
    splitBetween(regIndex, nonRegIndex, input, dummy, out2, 2, 7, false);
    splitBetween(regIndex, nonRegIndex + 1, input, dummy, out3, 0, 1, false);
    return 3;
  } else if (motifID == "E15") {
    return splitBetween(regIndex, nonRegIndex, input, out1, out2, 4, 64, false);
  }
  out1 = input;
  return 1;
}

/// Return that pad group type(s) corresponding to one single motifType
/// The number of pad group types is either 1 (most of cases), 2 (couple of cases) or 3 (a single case)
std::vector<PadGroupType>
getPadGroupTypes(int regIndex, int nonRegIndex, const rapidjson::Value &motifType, std::map<int, int> &berg2manu)
{
  const Value &pads = motifType["pads"];
  assert(pads.IsArray());
  std::string motifID = motifType["id"].GetString();

  unsigned int nofpads = pads.Size();

  PadGroupType v{regIndex, regIndex, motifID, false};

  for (const auto &p: pads.GetArray()) {
    assert(p["berg"].IsInt());
    assert(p["ix"].IsInt());
    assert(p["iy"].IsInt());
    int bergNumber = p["berg"].GetInt();
    auto it = berg2manu.find(bergNumber);
    if (it == berg2manu.end()) {
      std::cout << "did not find berg " << bergNumber << " ! for "
                << v.originalMotifTypeIdString
                << " " << berg2manu.size() << "\n";

      v.channelId.push_back(99);
    } else {
      v.channelId.push_back(it->second);
    }
    v.ix.push_back(p["ix"].GetInt());
    v.iy.push_back(p["iy"].GetInt());
  }

  PadGroupType v1, v2, v3;

  int n = split(regIndex, nonRegIndex, motifID, v, v1, v2, v3);

  if (n == 1) {
    return {v1};
  } else if (n == 2) {
    return {v1, v2};
  } else {
    return {v1, v2, v3};
  }
}

/// Return a map to convert berg pin number to manu channel
/// npins can only be 80 or 100
std::map<int, int> getBerg2Manu(const rapidjson::Value &berg, bool is80pins)
{
  std::map<int, int> b2m;
  int npins{is80pins ? 80 : 100};
  int ipin{0};
  if (berg[ipin]["id"].GetInt() != npins) { ipin = 1; }
  if (berg[ipin]["id"].GetInt() != npins) { throw std::runtime_error("number of pins can only be 80 or 100"); }

  for (const auto &b: berg[ipin]["pins"].GetArray()) {
    std::string smanu = b["manu"].GetString();
    if (isdigit(smanu[0])) {
      b2m[b["id"].GetInt()] = atoi(smanu.c_str());
    }
  }
  return b2m;
}

/// Return _all_ the pad group types
std::vector<PadGroupType> getPadGroupTypes(const rapidjson::Value &motiftypes, const rapidjson::Value &bergs)
{
  std::vector<PadGroupType> pgts;

  // the berg pin number to manu channel convertion is dependent on station (1,2 vs 3,4,5)
  std::array<std::map<int, int>, 2> berg2manuMaps = {getBerg2Manu(bergs, true), getBerg2Manu(bergs, false)};

  int nmotifs{static_cast<int>(motiftypes.GetArray().Size())};
  int nonRegIndex{nmotifs};
  std::map<int, int> m; // for non regular motifs gives the relation ship motifTypeId -> padGroupTypeId
  for (auto regIndex = 0; regIndex < nmotifs; ++regIndex) {
    const auto &motifType = motiftypes.GetArray()[regIndex];
    assert(motifType.IsObject());
    std::string motifTypeId = motifType["id"].GetString();
    std::map<int, int> *b2m{nullptr};
    if (motifTypeId[0] == '1' || motifTypeId[0] == '2') {
      b2m = &berg2manuMaps[0];
    } else {
      b2m = &berg2manuMaps[1];
    }
    std::vector<PadGroupType> padGroupTypes = getPadGroupTypes(regIndex, nonRegIndex, motifType, *b2m);
    nonRegIndex += padGroupTypes.size() - 1;
    pgts.insert(pgts.end(), padGroupTypes.begin(), padGroupTypes.end());
  }
  return pgts;
}

std::string generateCodeForPadGroupTypes(const rapidjson::Value &motiftypes, const rapidjson::Value &bergs)
{
  assert(motiftypes.IsArray());

  std::ostringstream impl;

  impl << generateInclude({"PadGroupType.h"});
  impl << mappingNamespaceBegin("impl2");
  impl << "PadGroupType getPadGroupType(int i) {\n";

  auto padGroupTypes = getPadGroupTypes(motiftypes, bergs);

  for (auto &padGroupType: padGroupTypes) {
    impl << padGroupType << "\n";
  }

  impl << "  throw std::out_of_range(\"i should be between 0 and "
       << padGroupTypes.size() << "\");\n";
  impl << "}\n";

  impl << mappingNamespaceEnd("impl2");

  return impl.str();
}

}
}
}
