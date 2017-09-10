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


#include "contour.h"
#include <stdexcept>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/is_empty.hpp>
#include <cmath>

namespace bg = boost::geometry;

namespace o2 {
namespace mch {
namespace geometry {

bool isLeftEdge(const VerticalEdge& ve)
{
  // a vertical ve with first point above the second one is a left edge
  return ve.yBegin > ve.yEnd;
}

bool isRightEdge(const VerticalEdge& ve)
{
  // a vertical ve with first point below the second one is a right edge
  return ve.yBegin < ve.yEnd;
}

double smallestY(const VerticalEdge& ve)
{

  return std::min(ve.yBegin, ve.yEnd);
}

void sortVerticalEdges(std::vector<VerticalEdge>& verticalEdges)
{
// sort vertical edges in ascending x order
// if same x, insure that left edges are before right edges
// within same x, order by increasing bottommost y
// Mind your steps ! This sorting is critical to the contour merging algorithm !

  std::sort(verticalEdges.begin(), verticalEdges.end(), [](const VerticalEdge& s1, const VerticalEdge& s2) {

    auto x1 = s1.x;
    auto x2 = s1.x;

    auto y1 = smallestY(s1);
    auto y2 = smallestY(s2);

    if (areEqual(x1, x2)) {
      if (isLeftEdge(s1) && isRightEdge(s2)) { return true; }
      if (isRightEdge(s1) && isLeftEdge(s2)) { return false; }
      return y1 < y2;
    } else if (x1 < x2) {
      return true;
    } else {
      return false;
    }
  });
}

std::vector<VerticalEdge> sweep(const std::vector<VerticalEdge>& polygonVerticalEdges)
{

  return {};
}

MultiPolygon createContour(MultiPolygon& polygons)
{
  MultiPolygon c;
  if (bg::is_empty(polygons)) {
    return c;
  }
  for (auto i = 0; i < polygons.size(); ++i) {
    if (!isCounterClockwiseOriented(polygons[i])) {
      throw std::invalid_argument("polygons should be oriented counterclockwise");
    }
  }

  // trivial case : only one input polygon
  if (polygons.size() == 1) {
    return polygons;
  }

  std::vector<VerticalEdge> polygonVerticalEdges{getVerticalEdges(polygons)};

  sortVerticalEdges(polygonVerticalEdges);

  std::vector<VerticalEdge> contourVerticalEdges{sweep(polygonVerticalEdges)};

//  // Find the vertical edges of the merged contour. This is the meat of the algorithm...
//  TObjArray contourVerticalEdges;
//  contourVerticalEdges.SetOwner(kTRUE);
//  Sweep(polygonVerticalEdges,contourVerticalEdges);
//
//  TObjArray horizontals;
//  horizontals.SetOwner(kTRUE);
//  VerticalToHorizontal(contourVerticalEdges,horizontals);
//
//  contour = FinalizeContour(contourVerticalEdges,horizontals);
//
//  if ( contour && name ) contour->SetName(name);
  return c;
}

double signedArea(const SimplePolygon& polygon)
{
  /// Compute the signed area of this polygon
  /// Algorithm from F. Feito, J.C. Torres and A. Urena,
  /// Comput. & Graphics, Vol. 19, pp. 595-600, 1995
  double area{0.0};
  for (auto i = 0; i < polygon.outer().size() - 1; ++i) {
    const Point& current = polygon.outer()[i];
    const Point& next = polygon.outer()[i + 1];
    area += current.x() * next.y() - next.x() * current.y();
  }
  return area * 0.5;
}

bool isCounterClockwiseOriented(const SimplePolygon& polygon)
{
  return signedArea(polygon) > 0.0;
}

std::vector<VerticalEdge> getVerticalEdges(const SimplePolygon& polygon)
{
  /// Return the vertical edges of the input polygon
  std::vector<VerticalEdge> verticals;
  for (auto i = 0; i < polygon.outer().size() - 1; ++i) {
    const Point& current = polygon.outer()[i];
    const Point& next = polygon.outer()[i + 1];
    if (areEqual(current.x(), next.x()))// ve is vertical
    {
      verticals.push_back({current.x(), current.y(), next.y()});
    }
  }
  return verticals;
}

bool areEqual(double a, double b)
{
  return std::fabs(b - a) < 1E-5; // 1E-5 cm = 0.1 micron
}

std::vector<VerticalEdge> getVerticalEdges(const MultiPolygon& polygons)
{
  /// From an array of polygons, extract the list of vertical edges.
  std::vector<VerticalEdge> verticals;
  for (auto&& p : polygons) {
    std::vector<VerticalEdge> v = getVerticalEdges(p);
    verticals.insert(verticals.end(), v.begin(), v.end());
  }
  return verticals;
}

std::vector<double> getUniqueVerticalPositions(const std::vector<VerticalEdge>& ves)
{
  std::vector<double> ypos;

  for (auto&& v: ves) {
    ypos.push_back(v.yBegin);
    ypos.push_back(v.yEnd);
  }

  std::sort(ypos.begin(),ypos.end());
  auto last = std::unique(ypos.begin(),ypos.end(),[](const double& a, const double& b) { return areEqual(a,b); });
  ypos.erase(last,ypos.end());
  return ypos;
}

bool areEqual(const VerticalEdge& a, const VerticalEdge& b)
{
  return areEqual(a.x, b.x) && areEqual(a.yBegin, b.yBegin) && areEqual(a.yEnd, b.yEnd);
}

}
}
}

