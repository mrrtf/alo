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
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

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

void Segmentation::fillRtree()
{
  //const double epsilon{1E-3}; // artificially decrease size of pads by 10 microns to avoid overlaps
  const double epsilon{1E-6}; // artificially increase size of pads by a smidge to avoid gaps
  //const double epsilon{0.0};

  mPads={};

  for (auto padGroupIndex =0; padGroupIndex < mPadGroups.size(); ++padGroupIndex) {
    auto& pg = mPadGroups[padGroupIndex];
    auto &pgt = mPadGroupTypes[pg.mPadGroupTypeId];
    double dx{mPadSizes[pg.mPadSizeId].first};
    double dy{mPadSizes[pg.mPadSizeId].second};
    for (int ix = 0; ix < pgt.getNofPadsX(); ++ix) {
      for (int iy = 0; iy < pgt.getNofPadsY(); ++iy) {
        if (pgt.id(ix, iy) >= 0) {

          double xmin = ix * dx + pg.mX - epsilon;
          double xmax = (ix + 1) * dx + pg.mX + epsilon;
          double ymin = iy * dy + pg.mY - epsilon;
          double ymax = (iy + 1) * dy + pg.mY + epsilon;

          mRtree.insert(std::make_pair(Segmentation::Box{
            Segmentation::Point(xmin, ymin),
            Segmentation::Point(xmax, ymax)
          }, mPads.size()));

          mPads.push_back(std::pair<int,int>(padGroupIndex,pgt.fastIndex(ix,iy)));

        }
      }
    }
  }
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
  mPads{}
{
  fillRtree();
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

std::ostream& Segmentation::showPad(std::ostream& out, int index) const {
  int padGroupIndex = mPads[index].first;
  int padChannel = mPads[index].second;
 auto& pg = mPadGroups[padGroupIndex];
  out << " [" << padGroupIndex << "," << padChannel << "] (FEC " << pg.mFECId << ")";
  return out;
}

bool Segmentation::hasPadByPosition(double x, double y) const
{
  return findPadByPosition(x,y) != -1;
}

int Segmentation::findPadByPosition(double x, double y) const
{
  static unsigned long one{0},moreThanOne{0};
  std::vector<Segmentation::Value> result_n;
  mRtree.query(boost::geometry::index::contains(Segmentation::Point(x,y)), std::back_inserter(result_n));
  //mRtree.query(boost::geometry::index::nearest(Segmentation::Point(x,y),1), std::back_inserter(result_n));
  if (result_n.size()>1)
  {
    moreThanOne++;
    std::cout << "moreThanOne in ppm " << 1E6*moreThanOne/one << "\n";
  }
  else {
    one++;
  }

  if ( result_n.size() > 0 ) {
    return result_n[0].second;
  }
  return -1;
}

int Segmentation::nofPads(const PadGroup &padGroup) const
{
  return mPadGroupTypes[padGroup.mPadGroupTypeId].getNofPads();
}

// Return the list of valid fastindices (somewhere between 0 and npadX*npadY-1)
// for a given padgroup
std::vector<int> Segmentation::padFastIndices(const PadGroup &padGroup) const
{
  return mPadGroupTypes[padGroup.mPadGroupTypeId].fastIndices();
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

double Segmentation::padPositionX(int padGroupIndex, int padIndex) const
{
  auto pg = mPadGroups[padGroupIndex];
  auto pgt = mPadGroupTypes[pg.mPadGroupTypeId];
  return pg.mX + (pgt.ix(padIndex) + 0.5) * mPadSizes[pg.mPadSizeId].first;
}

double Segmentation::padPositionY(int padGroupIndex, int padIndex) const
{
  auto pg = mPadGroups[padGroupIndex];
  auto pgt = mPadGroupTypes[pg.mPadGroupTypeId];
  return pg.mY + (pgt.iy(padIndex) + 0.5) * mPadSizes[pg.mPadSizeId].second;
}

double Segmentation::padSizeX(int padGroupIndex) const
{
  return mPadSizes[mPadGroups[padGroupIndex].mPadSizeId].first;
}

double Segmentation::padSizeY(int padGroupIndex) const
{
  return mPadSizes[mPadGroups[padGroupIndex].mPadSizeId].second;
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
