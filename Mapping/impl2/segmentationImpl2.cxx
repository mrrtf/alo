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
#include "padGroupType.h"
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

Segmentation::Polygon computePadGroupTypeContour(const PadGroupType &pgt, double padSizeX, double padSizeY)
{
  std::vector<o2::mch::contour::Polygon<double>> pads{computePads(pgt, padSizeX, padSizeY)};

  o2::mch::contour::Contour<double> contour = o2::mch::contour::createContour(pads);
  if (contour.size() != 1) {
    std::cout << "OUPS. Contour.size()=" << contour.size() << "\n";
    throw std::runtime_error("contour size should be 1 and is " + std::to_string(contour.size()));
  }
  if (!contour[0].isCounterClockwiseOriented()) {
    throw std::runtime_error("polygons must be counterclockwise oriented !");
  }
  return contour[0];
}

std::vector<Segmentation::Polygon> computeContours(const std::vector<PadGroup> &padGroups,
                                                   const std::vector<PadGroupType> &padGroupTypes,
                                                   const std::vector<std::pair<float, float>> &padSizes)
{
  std::vector<Segmentation::Polygon> contours;

  for (auto &pg: padGroups) {
    auto c = computePadGroupTypeContour(padGroupTypes[pg.mPadGroupTypeId], padSizes[pg.mPadSizeId].first,
                                        padSizes[pg.mPadSizeId].second);
    c.translate(pg.mX, pg.mY);
    contours.push_back(c);
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

Segmentation::Polygon computeEnvelop(const std::vector<Segmentation::Polygon> &polygons)
{
  auto contour = o2::mch::contour::createContour(polygons);
  if (contour.size() != 1) {
    throw std::runtime_error("envelop is not a single polygon !");
  }
  return contour[0];
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
  mMaxFastIndex{-1},
  mEnvelop{computeEnvelop(mPadGroupContours)},
  mBBox{o2::mch::contour::getBBox(mEnvelop)}
{
  for (auto &pgt: mPadGroupTypes) {
    for (auto &fi : pgt.fastIndices()) {
      mMaxFastIndex = std::max(mMaxFastIndex, fi);
    }
  }
  mMaxFastIndex++;
}

std::vector<int> Segmentation::findPadGroupIndices(double x, double y) const
{
  std::vector<int> pgi;
  for (auto i = 0; i < mPadGroupContours.size(); ++i) {
    if (mPadGroupContours[i].contains(x, y)) {
      pgi.push_back(i);
    }
  }
  return pgi;
}

int Segmentation::padUid(int padGroupIndex, double x, double y) const
{
  auto &pg = mPadGroups[padGroupIndex];
  int ix = static_cast<int>(std::floor((x - pg.mX) / mPadSizes[pg.mPadSizeId].first));
  int iy = static_cast<int>(std::floor((y - pg.mY) / mPadSizes[pg.mPadSizeId].second));
  auto &pgt = mPadGroupTypes[pg.mPadGroupTypeId];
  if (pgt.hasPadById(pgt.id(ix, iy))) {
    return padUid(padGroupIndex, pgt.fastIndex(ix, iy));
  }
  return -1;
}

int Segmentation::findPadByPosition(double x, double y) const
{
  auto pgis = findPadGroupIndices(x, y);
  if (pgis.empty()) {
    return -1;
  }

  // most probable case : only 1 padgroup found
  if (pgis.size() == 1) {
    return padUid(pgis.front(), x, y);
  }

  // more than one padgroup : find one
  // that give back a valid pad
  double dmin{std::numeric_limits<double>::max()};
  int rv{-1};
  o2::mch::contour::Vertex<double> point{x, y};

  for (auto i = 0; i < pgis.size() && rv < 0; ++i) {
    double d = o2::mch::contour::squaredDistancePointToPolygon(point, mPadGroupContours[pgis[i]]);
    int p = padUid(pgis[i], x, y);
    if (p >= 0) {
      rv = p;
    }
  }

  return rv;
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

std::vector<int> Segmentation::uniqueAndValid(std::vector<int> paduids) const
{
  std::vector<int> rv;
  std::sort(begin(paduids), end(paduids));
  int previous{InvalidPadUid};
  for (auto i = 0; i < paduids.size(); ++i) {
    int p = paduids[i];
    if (p != previous && p != InvalidPadUid) {
      rv.push_back(p);
      previous = p;
    }
  }
  return rv;
}


void dumpPads(std::string msg, const Segmentation &seg, const std::vector<int> &paduids)
{
  std::cout << msg << "\n";
  for (auto &paduid: paduids) {
    std::cout << boost::str(boost::format("Pad %10d FEC %4d CH %2d X %7.3f Y %7.3f SX %7.3f SY %7.3f\n") % paduid
                            % seg.padDualSampaId(paduid) % seg.padDualSampaChannel(paduid) % seg.padPositionX(paduid) %
                            seg.padPositionY(paduid)
                            % seg.padSizeX(paduid) % seg.padSizeY(paduid));
  }
}


/// Starting from {xstart,ystart} and moving towards {xend,ystart}
/// return the first valid paduid found
/// FIXME: very basic algorithm for the moment. There is certainly room for
/// improvement _if_ this method shows up at some point in performance
/// _measurements_
int Segmentation::findClosestX(double xstart, double ystart, double xend) const
{
  double xstep{0.1}; // cm

  if (xstart > xend) {
    for (auto x = xstart; x > xend; x -= xstep) {
      int paduid{findPadByPosition(x, ystart)};
      if (isValid(paduid)) {
        return paduid;
      }
    }
  } else {
    for (auto x = xstart; x < xend; x += xstep) {
      int paduid{findPadByPosition(x, ystart)};
      if (isValid(paduid)) {
        return paduid;
      }
    }

  }
  return InvalidPadUid;
}

int Segmentation::findClosestY(double xstart, double ystart, double yend) const
{
  double ystep{0.1}; // cm

  if (ystart > yend) {
    for (auto y = ystart; y > yend; y -= ystep) {
      int paduid{findPadByPosition(xstart, y)};
      if (isValid(paduid)) {
        return paduid;
      }
    }
  } else {
    for (auto y = ystart; y < yend; y += ystep) {
      int paduid{findPadByPosition(xstart, y)};
      if (isValid(paduid)) {
        return paduid;
      }
    }
  }
  return InvalidPadUid;
}

/// Return the corner pads of the intersection of box with this segmentation
std::vector<int> Segmentation::intersect(const BBox &box) const
{
  auto b = o2::mch::contour::intersect(box, mBBox);
  const double epsilon{2E-4};

  double xmin{b.xmin() + epsilon};
  double ymin{b.ymin() + epsilon};
  double xmax{b.xmax() - epsilon};
  double ymax{b.ymax() - epsilon};

  int topLeft = findPadByPosition(xmin, ymax);
  int bottomLeft = findPadByPosition(xmin, ymin);
  int bottomRight = findPadByPosition(xmax, ymin);
  int topRight = findPadByPosition(xmax, ymax);

  // some more work might be needed if some corner pads are not
  // within the segmentation bounding box.
  //
  // can happen when envelop is not the same as the bounding box, e.g.
  // for quadrants or rounded slats.
  //
  // in each case we sweep along an horizontal or vertical line
  // to find the first valid pad

  if (!isValid(topRight)) {
    topRight = findClosestX(xmax, ymax, xmin);
    if (!isValid(topRight)) {
      topRight = findClosestY(xmin, ymax, ymin);
    }
  }
  if (!isValid(bottomRight)) {
    bottomRight = findClosestX(xmax, ymin, xmin);
    if (!isValid(bottomRight)) {
      bottomRight = findClosestY(xmax, ymin, ymax);
    }
  }
  if (!isValid(topLeft)) {
    topLeft = findClosestX(xmin, ymax, xmax);
    if (!isValid(topLeft)) {
      topLeft = findClosestY(xmin, ymax, ymin);
    }
  }
  if (!isValid(bottomLeft)) {
    bottomLeft = findClosestX(xmin, ymin, xmax);
    if (!isValid(bottomLeft)) {
      bottomLeft = findClosestY(xmin, ymin, ymax);
    }
  }
  return {topLeft, bottomLeft, bottomRight, topRight};
}

std::vector<int> Segmentation::getPadUids(const BBox &area) const
{
  std::vector<int> corners = intersect(area);

  // use the smallest corner pads to decide on traversal steps in x and y
  double xstep{std::numeric_limits<double>::max()};
  double ystep{std::numeric_limits<double>::max()};

  double xstart{std::numeric_limits<double>::max()};
  double xend{std::numeric_limits<double>::lowest()};
  double ystart{std::numeric_limits<double>::max()};
  double yend{std::numeric_limits<double>::lowest()};

  std::vector<int> paduids;

  for (auto puid: corners) {
    if (isValid(puid)) {
      xstep = std::min(xstep, padSizeX(puid));
      ystep = std::min(ystep, padSizeY(puid));
      xstart = std::min(xstart, padPositionX(puid));
      xend = std::max(xend, padPositionX(puid));
      ystart = std::min(ystart, padPositionY(puid));
      yend = std::max(yend, padPositionY(puid));
      paduids.push_back(puid);
    }
  }

  dumpPads("--- corners", *this, paduids);
  std::cout << boost::format("x,y start = %7.2f %7.2f x,y end = %7.2f %7.2f x,y step = %7.2f %7.2f\n") %
               xstart % ystart % xend % yend % xstep % ystep;

  for (auto x = xstart; x < xend; x += xstep) {
    for (auto y = ystart; y < yend; y += ystep) {
      paduids.push_back(findPadByPosition(x, y));
    }
  }

  for (auto y = ystart; y < yend; y += ystep) {
    paduids.push_back(findPadByPosition(xend, y));
  }

  for (auto x = xstart; x < xend; x += xstep) {
    //paduids.push_back(findPadByPosition(x, yend));
    int p = findPadByPosition(x, ystart);
    if (isValid(p)) {
      paduids.push_back(p);
//      std::stringstream str;
//      str << boost::format("x %7.2f y %7.2f") % x % ystart;
//      dumpPads(str.str(),*this,{p});
    }
  }

  paduids.push_back(findPadByPosition(xend, yend));

  paduids = uniqueAndValid(paduids);
  dumpPads("--- pads", *this, paduids);
  return paduids;
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

