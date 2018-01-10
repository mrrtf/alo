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

#include "segmentationImpl2.h"
#include "genSegmentation.cxx"
#include "padGroup.h"
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>

using namespace o2::mch::mapping::impl2;

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

Segmentation::Contour computeContour(const PadGroup &pg, const PadGroupType &pgt)
{
  return Segmentation::Contour{};
}

std::vector<Segmentation::Contour> computeContours(const std::vector<PadGroup> &padGroups,
                                                   const std::vector<PadGroupType> &padGroupTypes)
{
  std::vector<Segmentation::Contour> contours;
  for (auto &pg : padGroups) {
    contours.push_back(computeContour(pg, padGroupTypes[pg.mPadGroupTypeId]));
  }
  return contours;
}

using PadGroupFunc = int (*)(const PadGroup &);

int fecId(const PadGroup &padGroup)
{ return padGroup.mFECId; }

int groupTypeId(const PadGroup &padGroup)
{ return padGroup.mPadGroupTypeId; }

std::set<int> getUnique(const std::vector<PadGroup> &padGroups, PadGroupFunc func)
{
  // extract from padGroup vector the unique integer values given by func
  std::set<int> u;
  for (auto &pg: padGroups) {
    u.insert(func(pg));
  }
  return u;
}

std::vector<PadGroupType> getPadGroupTypes(const std::vector<PadGroup> &padGroups)
{
  // get a vector of unique padGroupTypes from padGroups
  std::vector<PadGroupType> padGroupTypes;
  std::set<int> padGroupTypeIds = getUnique(padGroups, groupTypeId);
  for (auto &pgt: padGroupTypeIds) {
    padGroupTypes.push_back(getPadGroupType(pgt));
  }
  return padGroupTypes;
}

std::vector<PadGroup> remap(const std::vector<PadGroup> &padGroups,
                            const std::vector<PadGroupType> &padGroupTypes)
{
  // change the padGroupType ids in padGroups vector to
  // be from 0 to number of different padgrouptypes in padGroups vector

  std::map<int, int> idmap;
  std::vector<PadGroup> remappedPadGroups;

  std::set<int> padGroupTypeIds = getUnique(padGroups, groupTypeId);

  int i{0};
  for (auto& pgt: padGroupTypeIds) {
    idmap[pgt] = i++;
  }

  for (auto &pg: padGroups) {
    auto it = idmap.find(pg.mPadGroupTypeId);
    if (it == idmap.end()) { throw; }
    PadGroup rpg{pg};
    rpg.mPadGroupTypeId = it->second;
    remappedPadGroups.push_back(rpg);
  }

  return remappedPadGroups;
}

Segmentation::Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups) :
  mSegType{segType},
  mIsBendingPlane{isBendingPlane},
  mDualSampaIds{getUnique(padGroups, fecId)},
  mPadGroupTypes{getPadGroupTypes(padGroups)},
  mPadGroups{remap(padGroups, mPadGroupTypes)},
  mPadGroupContours{computeContours(mPadGroups, mPadGroupTypes)}
{
}

std::vector<int> Segmentation::padGroupIndices(int dualSampaId) const
{
  std::vector<int> rv;
  for (auto i = 0; i < mPadGroups.size(); ++i) {
    if (mPadGroups[i].mFECId == dualSampaId) {
      rv.push_back(i);
    }
  }
  return rv;
}

bool Segmentation::hasPadByPosition(double x, double y) const
{
  return false;
}

int Segmentation::nofPads(const PadGroup &padGroup) const
{
  return mPadGroupTypes[padGroup.mPadGroupTypeId].getNofPads();
}


bool Segmentation::hasPadByFEE(int dualSampaId, int dualSampaChannel) const
{
  bool rv{false};
  for (auto index: padGroupIndices(dualSampaId)) {
    auto &pg = mPadGroups[index];
    auto &pgt = mPadGroupTypes[pg.mPadGroupTypeId];
    if (pgt.hasPadById(dualSampaChannel)) {
      rv = true;
    }
  }
  return rv;
}

}
}
}
}

