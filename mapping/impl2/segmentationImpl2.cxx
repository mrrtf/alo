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
#include "genDetElemId2SegType.h"
#include "padGroup.h"
#include "padSize.h"
#include "segmentation.h"
#include "segmentationCreator.h"
#include "contourCreator.h"
#include "padGroupTypeContour.h"
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <gsl/gsl>

using namespace o2::mch::mapping::impl2;

namespace o2 {
namespace mch {
namespace mapping {
namespace impl2 {

Segmentation *createSegmentation(int detElemId, bool isBendingPlane)
{
  int segType = detElemId2SegType(detElemId);
  SegmentationCreator creator = getSegmentationCreator(segType);
  if (creator == nullptr) {
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

  std::vector<o2::mch::contour::Polygon<double>> pgtContours = computeContours(padGroupTypes);

  std::vector<Segmentation::Contour> contours;

  for (auto &pg: padGroups) {
    auto p = pgtContours[pg.mPadGroupTypeId];
    double dx{padSizes[pg.mPadSizeId].first};
    double dy{padSizes[pg.mPadSizeId].second};
    p.scale(dx, dy);
    p.translate(pg.mX, pg.mY);
    if (!p.isCounterClockwiseOriented()) {
      std::cout << p << "\n";
      throw std::runtime_error("polygons must be counterclockwise oriented !");
    }
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
                           std::vector<std::pair<float, float>> padSizes)
  :
  mSegType{segType},
  mIsBendingPlane{isBendingPlane},
  mPadGroups{std::move(padGroups)},
  mDualSampaIds{getUnique(mPadGroups)},
  mPadGroupTypes{std::move(padGroupTypes)},
  mPadSizes{std::move(padSizes)},
  mPadGroupContours{computeContours(mPadGroups, mPadGroupTypes, mPadSizes)},
  mMaxFastIndex{-1}
{
  for (auto& pgt: mPadGroupTypes)
  {
    for (auto& fi : pgt.fastIndices()) {
      mMaxFastIndex = std::max(mMaxFastIndex, fi);
    }
  }
  mMaxFastIndex++;
}

int Segmentation::findPadGroupIndex(double x, double y) const
{
  for (auto i = 0; i < mPadGroupContours.size(); ++i  ){
    if (mPadGroupContours[i].contains(x, y)) {
      return i;
    }
  }
  return -1;
}

int Segmentation::findPadByPosition(double x, double y) const
{
  int pgi = findPadGroupIndex(x,y);
  if (pgi < 0 )
  {
    return -1;
  }

  auto& pg = mPadGroups[pgi];
  int ix = (x - pg.mX)/mPadSizes[pg.mPadSizeId].first;
  int iy = (y - pg.mY)/mPadSizes[pg.mPadSizeId].second;
  auto& pgt = mPadGroupTypes[pg.mPadGroupTypeId];
  if ( pgt.hasPadById(pgt.id(ix,iy))) {
    return padUid(pgi,pgt.fastIndex(ix,iy));
  }
  return -1;
}

std::vector<int> Segmentation::getPadUids(int dualSampaId) const
{
  std::vector<int> pi;

  for (auto padGroupIndex = 0; padGroupIndex < mPadGroups.size(); ++padGroupIndex) {
    if (mPadGroups[padGroupIndex].mFECId == dualSampaId) {
      auto &pgt = mPadGroupTypes[mPadGroups[padGroupIndex].mPadGroupTypeId];
      for (auto& fi: pgt.fastIndices()) {
        pi.push_back(padUid(padGroupIndex,fi));
      }
    }
  }

  return pi;
}
const PadGroup &Segmentation::padGroup(int paduid) const
{
  return gsl::at(mPadGroups, padUid2padGroupIndex(paduid));
}

const PadGroupType &Segmentation::padGroupType(int paduid) const
{
  return gsl::at(mPadGroupTypes, padGroup(paduid).mPadGroupTypeId);
}

int Segmentation::findPadByFEE(int dualSampaId, int dualSampaChannel) const
{
  for (auto paduid: getPadUids(dualSampaId)) {
    if (padGroupType(paduid).hasPadById(dualSampaChannel)) {
      return paduid;
    }
  }
  return InvalidPadUid;
}

double Segmentation::padPositionX(int paduid) const
{
  auto &pg = padGroup(paduid);
  auto &pgt = padGroupType(paduid);
  return pg.mX + (pgt.ix(padUid2padGroupTypeFastIndex(paduid)) + 0.5) * mPadSizes[pg.mPadSizeId].first;
}

double Segmentation::padPositionY(int paduid) const
{
  auto &pg = padGroup(paduid);
  auto &pgt = padGroupType(paduid);
  return pg.mY + (pgt.iy(padUid2padGroupTypeFastIndex(paduid)) + 0.5) * mPadSizes[pg.mPadSizeId].second;
}

double Segmentation::padSizeX(int paduid) const
{
  return mPadSizes[padGroup(paduid).mPadSizeId].first;
}

double Segmentation::padSizeY(int paduid) const
{
  return mPadSizes[padGroup(paduid).mPadSizeId].second;
}

std::ostream &operator<<(std::ostream &out, const std::pair<float, float> &p)
{
  out << p.first << "," << p.second;
  return out;
}

template<typename T>
void dump(std::ostream &out, const std::string &msg, const std::vector<T> &v, int n)
{

  out << msg << "\n";
  for (auto i = 0; i < n; ++i) {
    if (i < v.size()) {
      out << v[i] << "\n";
    }
  }
}

std::ostream &operator<<(std::ostream &os, const Segmentation &seg)
{
  os << "segType " << seg.mSegType << "-" << (seg.mIsBendingPlane ? "B" : "NB");

  os << boost::format(" %3d PG %2d PGT %2d PS\n") % seg.mPadGroups.size() % seg.mPadGroupTypes.size() %
        seg.mPadSizes.size();

  dump(os, "PG", seg.mPadGroups, seg.mPadGroups.size());
  dump(os, "PGT", seg.mPadGroupTypes, seg.mPadGroupTypes.size());
  dump(os, "PS", seg.mPadSizes, seg.mPadSizes.size());
  return os;
}

}
}
}
}

