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

#include "padGroup.h"
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

namespace jsonmap {
namespace codegen {
namespace impl2 {

std::ostream &operator<<(std::ostream &os, const PadGroup &pg)
{
  os << "{" << pg.fecId << "," << pg.padGroupTypeId << "," << pg.padSizeId << "," << pg.x << "," << pg.y << "}";
  return os;
}

void getPosition(const MotifPosition &mp, const PadGroupType &pgt, double &x, double &y)
{
  // once again, don't fret about some hard-coded stuff, that's faster sometimes ;-)

  x = mp.mPositionX;
  y = mp.mPositionY;

  using shifts = std::tuple<std::string, int, double, double>;

  //                            motiftype, motiftypeid, shift in x, shift in y (cm)
  std::vector<shifts> special = {{"1BG", 16, 2.52 * 3, 0.0},
                                 {"1NG", 16, 0.0,      1.68 * 3},
                                 {"1NH", 12, 0.0,      1.68 * 3},
                                 {"E14", 7,  1 * 2.5,  10 * 0.5},
                                 {"E15", 4,  1 * 2.5,  5 * 0.5}};

  for (auto &s : special) {
    if (pgt.originalMotifTypeIdString == std::get<0>(s) &&
        pgt.channelId.size() == std::get<1>(s)) {
      x += std::get<2>(s);
      y += std::get<3>(s);
    }
  }
}


int getPadSizeId(const MotifPosition &mp, const PadGroupType &pgt, int channel,
                 const std::map<int, int> &manu2berg)
{
  if (mp.hasTwoPadSizes()) {
    int berg = manu2berg.find(channel)->second;
    auto it = std::find(mp.mPadNumbers.begin(), mp.mPadNumbers.end(), berg);
    if (it != mp.mPadNumbers.end()) {
      return mp.mSecondPadSizeId;
    }
  }
  return mp.mPadSizeId;
}

using PadGroupFunc = int (*)(const PadGroup &);

int fecId(const PadGroup &padGroup)
{ return padGroup.fecId; }

int groupTypeId(const PadGroup &padGroup)
{ return padGroup.padGroupTypeId; }

int padSizeId(const PadGroup &padGroup)
{
  return padGroup.padSizeId;
}

std::set<int> getUnique(const std::vector<PadGroup> &padGroups, PadGroupFunc func)
{
  // extract from padGroup vector the unique integer values given by func
  std::set<int> u;
  for (auto &pg: padGroups) {
    u.insert(func(pg));
  }
  return u;
}

PadGroupType getPadGroupType(const std::vector<PadGroupType>& padGroupTypes, int i) {

  auto it = std::find_if(begin(padGroupTypes),end(padGroupTypes),
                         [&i](const PadGroupType& pgt) { return pgt.id == i; });
  return *it;
}

std::vector<PadGroupType>
getPadGroupTypes(const std::vector<PadGroup> &padGroups, const std::vector<PadGroupType> &padGroupTypes)
{
  // get a vector of unique padGroupTypes from padGroups

  std::vector<PadGroupType> pgts;
  std::set<int> padGroupTypeIds = getUnique(padGroups, groupTypeId);
  for (auto &i: padGroupTypeIds) {
    pgts.push_back(getPadGroupType(padGroupTypes,i));
  }
  return pgts;
}

std::vector<std::pair<float, float>>
getPadSizes(const std::vector<PadGroup> &padGroups, const std::vector<std::pair<float, float>> &allPadSizes)
{
  std::set<int> padSizeIds = getUnique(padGroups, padSizeId);

  std::vector<std::pair<float, float>> padSizes;
  for (auto &sid: padSizeIds) {
    padSizes.push_back(allPadSizes[sid]);
  }
  return padSizes;
}

std::vector<PadGroup>
getPadGroups(const std::vector<MotifPosition> &motifPositions, const std::vector<PadGroupType> &padGroupTypes,
             const std::map<int, int> &manu2berg)
{
  std::vector<PadGroup> pgs;
  for (auto i = 0; i < motifPositions.size(); ++i) {
    const auto &mp = motifPositions[i];
    for (auto &pgt : padGroupTypes) {
      if (pgt.originalMotifTypeId == mp.mMotifTypeId) {
        int padSizeId{getPadSizeId(mp, pgt, pgt.channelId[0], manu2berg)};
        double x, y;
        getPosition(mp, pgt, x, y);
        pgs.push_back({mp.mFECId, pgt.id, padSizeId, static_cast<float>(x), static_cast<float>(y)});
      }
    }
  }
  return pgs;
}

std::vector<PadGroup> remap(const std::vector<PadGroup> &padGroups,
                            const std::vector<PadGroupType> &padGroupTypes,
                            const std::vector<std::pair<float, float>> &padSizes)
{
  // change the padGroupType ids in padGroups vector to
  // be from 0 to number of different padgrouptypes in padGroups vector
  std::map<int, int> idmap;
  std::map<int, int> sizemap;
  std::vector<PadGroup> remappedPadGroups;

  std::set<int> padGroupTypeIds = getUnique(padGroups, groupTypeId);
  std::set<int> padSizeIds = getUnique(padGroups, padSizeId);

  int i{0};
  for (auto &pgt: padGroupTypeIds) {
    idmap[pgt] = i++;
  }

  i = 0;
  for (auto &ps: padSizeIds) {
    sizemap[ps] = i++;
  }

  for (auto &pg: padGroups) {
    auto it = idmap.find(pg.padGroupTypeId);
    if (it == idmap.end()) {
      throw std::runtime_error("pad group id " + std::to_string(pg.padGroupTypeId) + " not found");
    }
    auto sit = sizemap.find(pg.padSizeId);
    if (sit == sizemap.end()) {
      throw std::runtime_error("pad size id " + std::to_string(pg.padSizeId) + " not found");
    }
    PadGroup rpg{pg};
    rpg.padGroupTypeId = it->second;
    rpg.padSizeId = sit->second;
    remappedPadGroups.push_back(rpg);
  }

  return remappedPadGroups;
}

}
}
}
