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
#include "boost/format.hpp"
#include "contourCreator.h"
#include "genDetElemId2SegType.h"
#include "padGroup.h"
#include "padGroupTypeContour.h"
#include "padSize.h"
#include "polygon.h"
#include "segmentation.h"
#include "segmentationCreator.h"
#include "svgContour.h"
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using namespace o2::mch::mapping::impl2;
using namespace o2::mch::contour;

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

Segmentation *createSegmentation(int detElemId, bool isBendingPlane)
{
  int segType = detElemId2SegType(detElemId);
  SegmentationCreator creator = getSegmentationCreator(segType);
  if (!creator) {
    return nullptr;
  }
  return creator(isBendingPlane);
}

std::vector<Segmentation::Contour> computeContours(const std::vector<PadGroup> &padGroups,
                                                   const std::vector<PadGroupType> &padGroupTypes,
                                                   const std::vector<std::pair<float, float>> &padSizes)
{
  //std::cout << boost::format("computeContours %3d padgroups %2d padgrouptypes %2d padsizes\n")
  //             % padGroups.size() % padGroupTypes.size() % padSizes.size();

  std::vector<o2::mch::contour::Polygon<int>> pgtContours = computeContours(padGroupTypes);

  std::vector<Segmentation::Contour> contours;

  for (auto &pg: padGroups) {
    auto p = pgtContours[pg.mPadGroupTypeId].convert(1.0);
    float dx{padSizes[pg.mPadSizeId].first};
    float dy{padSizes[pg.mPadSizeId].second};
    p.scale(dx, dy);
    p.translate(pg.mX, pg.mY);
    contours.push_back(p);
  }

  return contours;
}

std::set<int> getUnique(const std::vector<PadGroup> &padGroups)
{
  // extract from padGroup vector the unique integer values given by func
  std::set<int> u;
  for (auto &pg: padGroups) {
    u.insert(pg.mFECId);
  }
  return u;
}

Segmentation::Segmentation(int segType, bool isBendingPlane, std::vector<PadGroup> padGroups,
                           std::vector<PadGroupType> padGroupTypes,
                           std::vector<std::pair<float,float>> padSizes)
:
  mSegType{segType},
  mIsBendingPlane{isBendingPlane},
  mDualSampaIds{getUnique(padGroups)},
  mPadGroupTypes{padGroupTypes},
  mPadSizes{padSizes},
  mPadGroups{padGroups},
  mPadGroupContours{computeContours(mPadGroups, mPadGroupTypes, mPadSizes)}
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

std::ostream& operator<<(std::ostream& out, const std::pair<float,float>& p)
{
  out << p.first << "," << p.second;
  return out;
}

template<typename T>
void dump(std::ostream& out, std::string msg, const std::vector<T>& v, int n) {

  out << msg << "\n";
  for ( auto i = 0; i < n; ++i ) {
    if (i<v.size()) {
      out << v[i] << "\n";
    }
  }
}

std::ostream& operator<<(std::ostream &os, const Segmentation &seg)
{
  os << "segType " << seg.mSegType << "-" << (seg.mIsBendingPlane ? "B":"NB");

  os << boost::format(" %3d PG %2d PGT %2d PS\n") % seg.mPadGroups.size() % seg.mPadGroupTypes.size() % seg.mPadSizes.size();

  dump(os,"PG",seg.mPadGroups,seg.mPadGroups.size());
  dump(os,"PGT",seg.mPadGroupTypes,seg.mPadGroupTypes.size());
  dump(os,"PS",seg.mPadSizes,seg.mPadSizes.size());
  return os;
}

}
}
}
}

