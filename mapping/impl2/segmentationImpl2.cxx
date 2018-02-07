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
  // extract from padGroup vector the unique FECids
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
  for (auto &pgt: mPadGroupTypes) {
    for (auto &fi : pgt.fastIndices()) {
      mMaxFastIndex = std::max(mMaxFastIndex, fi);
    }
  }
  mMaxFastIndex++;
}

int Segmentation::findPadGroupIndex(double x, double y) const
{
  for (auto i = 0; i < mPadGroupContours.size(); ++i) {
    if (mPadGroupContours[i].contains(x, y)) {
      return i;
    }
  }
  return -1;
}

int Segmentation::findPadByPosition(double x, double y) const
{
  int pgi = findPadGroupIndex(x, y);
  if (pgi < 0) {
    std::cout << boost::format("did not find pgi for %f %f\n") % x % y;
    return -1;
  }

  auto &pg = mPadGroups[pgi];
  int ix = static_cast<int>(std::floor((x - pg.mX) / (mPadSizes[pg.mPadSizeId].first + COMPUTEPADPRECISION)));
  int iy = static_cast<int>(std::floor((y - pg.mY) / (mPadSizes[pg.mPadSizeId].second + COMPUTEPADPRECISION)));
  auto &pgt = mPadGroupTypes[pg.mPadGroupTypeId];
  if (pgt.hasPadById(pgt.id(ix, iy))) {
    return padUid(pgi, pgt.fastIndex(ix, iy));
  }
  std::cout << boost::format("did not find pad in pgt %d %d  = %d for %f %f\n") % ix % iy % pgt.id(ix,iy) % x % y;
  return -1;
}

std::vector<int> Segmentation::getPadUids(int dualSampaId) const
{
  std::vector<int> pi;

  for (auto padGroupIndex = 0; padGroupIndex < mPadGroups.size(); ++padGroupIndex) {
    if (mPadGroups[padGroupIndex].mFECId == dualSampaId) {
      auto &pgt = mPadGroupTypes[mPadGroups[padGroupIndex].mPadGroupTypeId];
      for (auto &fi: pgt.fastIndices()) {
        pi.push_back(padUid(padGroupIndex, fi));
      }
    }
  }

  return pi;
}

std::vector<int> Segmentation::getPadUids(double xmin, double ymin, double xmax, double ymax) const
{
  double xstep = mPadSizes[0].first; // here we use the fact that pad sizes are guaranteed to be ordered by size
  double ystep = mPadSizes[0].second;

  std::vector<int> paduids;

  for (double x = xmin; x <= xmax; x += xstep) {
    for (double y = ymin; y <= ymax; y += ystep) {
      int paduid = findPadByPosition(x, y);
      if (paduid != InvalidPadUid) {
        paduids.push_back(paduid);
      }
    }
  }

  std::vector<int> rv;
  std::sort(begin(paduids),end(paduids));
  std::unique_copy(begin(paduids),end(paduids),std::back_inserter(rv));
  return rv;
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
    if (padGroupType(paduid).id(padUid2padGroupTypeFastIndex(paduid)) == dualSampaChannel) {
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

int Segmentation::padDualSampaId(int paduid) const
{
  return padGroup(paduid).mFECId;
}

int Segmentation::padDualSampaChannel(int paduid) const
{
  return padGroupType(paduid).id(padUid2padGroupTypeFastIndex(paduid));
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

